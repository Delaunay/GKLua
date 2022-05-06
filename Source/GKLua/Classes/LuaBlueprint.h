// BSD 3-Clause License Copyright (c) 2022, Pierre Delaunay All rights reserved.

#pragma once

// GKLua
#include "LuaBlueprintGeneratedClass.h"
#include "GKLuaScript.h"

// UnrealEngine
#include "Engine/Blueprint.h"

// Generated
#include "LuaBlueprint.generated.h"

/**
 * The Script blueprint generates script-defined classes
 * 
 * Bridge between the lua script and blueprint,
 * both exist at the same time
 */
UCLASS(BlueprintType)
class GKLUA_API ULuaBlueprint : public UBlueprint
{
	GENERATED_UCLASS_BODY()
public:


public:
	//! Reference ot the lua script that drives this blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UGKLuaScript *Script;

#if WITH_EDITOR
	virtual UClass *GetBlueprintClass() const override;

	void UpdateScriptStatus();;

	virtual bool SupportedByDefaultBlueprintFactory() const override
	{
		return false;
	}

	static bool ValidateGeneratedClass(const UClass *InClass);
#endif

	// COuld we override those to point to lua
	// 
	// TArray< UEdGrap...  	EventGraphs
	// TArray< UEdGrap...  	FunctionGraphs
};