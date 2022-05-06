// BSD 3-Clause License Copyright (c) 2022, Pierre Delaunay All rights reserved.

#include "LuaBlueprint.h"
#include "ScriptBlueprintEditor.h"

// UnrealEngine
#include "SKismetInspector.h"
#include "BlueprintEditorModes.h"

#define LOCTEXT_NAMESPACE "GKLuaEditor"

FScriptBlueprintEditor::FScriptBlueprintEditor()
{
}

FScriptBlueprintEditor::~FScriptBlueprintEditor()
{
	ULuaBlueprint *Blueprint = GetScriptBlueprintObj();
	if (Blueprint)
	{
		Blueprint->OnChanged().RemoveAll(this);
	}
}

void FScriptBlueprintEditor::InitScriptBlueprintEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost> &InitToolkitHost, const TArray<UBlueprint *> &InBlueprints, bool bShouldOpenInDefaultsMode)
{
	InitBlueprintEditor(Mode, InitToolkitHost, InBlueprints, bShouldOpenInDefaultsMode);
	SetCurrentMode(FBlueprintEditorApplicationModes::BlueprintDefaultsMode);
}

void FScriptBlueprintEditor::AddApplicationMode(FName ModeName, TSharedRef<FApplicationMode> Mode)
{
	if (ModeName == FBlueprintEditorApplicationModes::BlueprintDefaultsMode)
	{
		FBlueprintEditor::AddApplicationMode(ModeName, Mode);
	}
}

ULuaBlueprint *FScriptBlueprintEditor::GetScriptBlueprintObj() const
{
	return Cast<ULuaBlueprint>(GetBlueprintObj());
}

UBlueprint *FScriptBlueprintEditor::GetBlueprintObj() const
{
	auto Blueprint = FBlueprintEditor::GetBlueprintObj();
	// auto ScriptBlueprint = Cast<ULuaBlueprint>(Blueprint);
	// ScriptBlueprint->
	return Blueprint;
}

#undef LOCTEXT_NAMESPACE
