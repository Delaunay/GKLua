// BSD 3-Clause License Copyright (c) 2022, Pierre Delaunay All rights reserved.

// GKLua
#include "GKLuaLog.h"
#include "LuaBlueprint.h"

// Unreal Engine
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#if WITH_EDITOR
#include "EditorFramework/AssetImportData.h"
#include "Kismet2/BlueprintEditorUtils.h"
#endif

/////////////////////////////////////////////////////
// ULuaBlueprint

ULuaBlueprint::ULuaBlueprint(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	AssetImportData = CreateEditorOnlyDefaultSubobject<UAssetImportData>(TEXT("AssetImportData"));
#endif
}

#if WITH_EDITORONLY_DATA
void ULuaBlueprint::GetAssetRegistryTags(TArray<FAssetRegistryTag> &OutTags) const
{
	if (AssetImportData)
	{
		OutTags.Add(FAssetRegistryTag(SourceFileTagName(), AssetImportData->GetSourceData().ToJson(), FAssetRegistryTag::TT_Hidden));
	}

	Super::GetAssetRegistryTags(OutTags);
}
void ULuaBlueprint::PostLoad()
{
	Super::PostLoad();
	if (!SourceFilePath.IsEmpty() && AssetImportData)
	{
		AssetImportData->UpdateFilenameOnly(SourceFilePath);
	}
}
#endif

#if WITH_EDITOR
UClass *ULuaBlueprint::GetBlueprintClass() const
{
	return ULuaBlueprintGeneratedClass::StaticClass();
}

bool ULuaBlueprint::ValidateGeneratedClass(const UClass *InClass)
{
	const ULuaBlueprintGeneratedClass *GeneratedClass = Cast<const ULuaBlueprintGeneratedClass>(InClass);
	if (!ensure(GeneratedClass))
	{
		return false;
	}
	const ULuaBlueprint *Blueprint = Cast<ULuaBlueprint>(GetBlueprintFromClass(GeneratedClass));
	if (!ensure(Blueprint))
	{
		return false;
	}

	return true;
}

bool ULuaBlueprint::IsCodeDirty() const
{
	if (!AssetImportData)
	{
		return true;
	}

	const TArray<FAssetImportInfo::FSourceFile> &Data = AssetImportData->SourceData.SourceFiles;

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

void ULuaBlueprint::UpdateScriptStatus()
{
	if (IsCodeDirty())
	{
		FBlueprintEditorUtils::MarkBlueprintAsModified(this);
	}
}

void ULuaBlueprint::UpdateSourceCodeIfChanged()
{
	if (IsCodeDirty() && AssetImportData)
	{
		FString NewScript;
		FString Filename = AssetImportData->GetFirstFilename();
		if (FFileHelper::LoadFileToString(NewScript, *Filename))
		{
			SourceCode = NewScript;
			AssetImportData->Update(Filename);
		}
	}
}
#endif
