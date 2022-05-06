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

void ULuaBlueprint::UpdateScriptStatus()
{
	if (Script && Script->IsCodeDirty())
	{
		FBlueprintEditorUtils::MarkBlueprintAsModified(this);
	}
}
#endif
