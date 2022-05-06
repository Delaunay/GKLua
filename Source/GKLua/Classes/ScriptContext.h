// BSD 3-Clause License Copyright (c) 2022, Pierre Delaunay All rights reserved.
#pragma once

// GKLua
#include "LuaBlueprintGeneratedClass.h"

// UnrealEgnine
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UObject/ScriptMacros.h"
#include "Engine/BlueprintGeneratedClass.h"

// Generated
#include "ScriptContext.generated.h"

/**
 * Script context subobject
 */
UCLASS(DefaultToInstanced)
class GKLUA_API UScriptContext : public UObject
{
	GENERATED_UCLASS_BODY()

	virtual void PostInitProperties() override;
	virtual void BeginDestroy() override;

	UFUNCTION(BlueprintCallable, Category = "ScriptContext")
	void CallScriptFunction(FString FunctionName);

protected:
	/** Script context (code) */
	FScriptContextBase *Context;
};