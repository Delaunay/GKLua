// BSD 3-Clause License Copyright (c) 2022, Pierre Delaunay All rights reserved.


#include "GKLuaScript.h"

// Unreal Engine
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#if WITH_EDITOR
#include "EditorFramework/AssetImportData.h"
#include "Kismet2/BlueprintEditorUtils.h"
#endif

#ifdef WITH_LUA
extern "C"
{
#define LUA_COMPAT_APIINTCASTS 1
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#endif

UGKLuaScript::UGKLuaScript(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	AssetImportData = CreateEditorOnlyDefaultSubobject<UAssetImportData>(
		TEXT("AssetImportData")
	);
#endif
}

#if WITH_EDITORONLY_DATA
void UGKLuaScript::GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const
{
	if (AssetImportData)
	{
		OutTags.Add(FAssetRegistryTag(
			SourceFileTagName(),
			AssetImportData->GetSourceData().ToJson(), 
			FAssetRegistryTag::TT_Hidden)
		);
	}

	Super::GetAssetRegistryTags(OutTags);
}
void UGKLuaScript::PostLoad()
{
	Super::PostLoad();
	if (!LuaScriptPath.IsEmpty() && AssetImportData)
	{
		AssetImportData->UpdateFilenameOnly(GetScriptPath());
	}
}
#endif


#if WITH_EDITOR
bool UGKLuaScript::IsCodeDirty() const
{
	if (!AssetImportData)
	{
		return true;
	}

	const TArray<FAssetImportInfo::FSourceFile>& Data = AssetImportData->SourceData.SourceFiles;

	if (Data.Num() == 1)
	{
		// Check the timestamp of the file as it is now, and the last imported timestamp
		if (IFileManager::Get().GetTimeStamp(*AssetImportData->GetFirstFilename()) <= Data[0].Timestamp)
		{
			return false;
		}
	}

	return true;
}


void UGKLuaScript::UpdateSourceCodeIfChanged()
{
	if (IsCodeDirty() && AssetImportData)
	{
		FString NewScript;
		FString Filename = AssetImportData->GetFirstFilename();
		if (FFileHelper::LoadFileToString(NewScript, *Filename))
		{
			LuaSourceCode = NewScript;
			AssetImportData->Update(Filename);

			OnCodeSourceChanged.Broadcast();
		}
	}
}
#endif

void UGKLuaScript::Compile() {
#ifdef WITH_LUA
	bool bDebug = false;

	lua_State* LuaState = lua_open();

	FString Code;
	FFileHelper::LoadFileToString(Code, *GetScriptPath());

	// load a null terminated string
	luaL_loadstring(LuaState, TCHAR_TO_ANSI(*Code));

	auto Writer = [](lua_State* l, const void* p, size_t size, void* userdata) -> int {
		TArray<uint8>* Buffer = (TArray<uint8>*)userdata;
		Buffer->Append((uint8*)p, size);
	};

	lua_dump(LuaState, Writer, (void*)(&ByteCote), !bDebug);
#endif
}
