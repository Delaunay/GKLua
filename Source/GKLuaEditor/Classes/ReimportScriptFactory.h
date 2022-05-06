// BSD 3-Clause License Copyright (c) 2022, Pierre Delaunay All rights reserved.

//=============================================================================
// ReimportTextureFactory
//=============================================================================

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "ScriptFactory.h"
#include "EditorReimportHandler.h"

// Generated
#include "ReimportScriptFactory.generated.h"

/**
 * Script Blueprint re-import factory
 */
UCLASS(MinimalApi, collapsecategories, EarlyAccessPreview)
class UReimportScriptFactory : public UScriptFactory, public FReimportHandler
{
	GENERATED_UCLASS_BODY()

	UPROPERTY()
	class ULuaBlueprint *OriginalScript;

	virtual bool ConfigureProperties() override;

	//~ Begin FReimportHandler interface
	virtual bool CanReimport(UObject *Obj, TArray<FString> &OutFilenames) override;
	virtual void SetReimportPaths(UObject *Obj, const TArray<FString> &NewReimportPaths) override;
	virtual EReimportResult::Type Reimport(UObject *Obj) override;
	virtual int32 GetPriority() const override;
	//~ End FReimportHandler interface
};
