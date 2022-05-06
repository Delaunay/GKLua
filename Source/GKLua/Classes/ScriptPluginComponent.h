// BSD 3-Clause License Copyright (c) 2022, Pierre Delaunay All rights reserved.

#pragma once

// GKLua
#include "LuaBlueprintGeneratedClass.h"

// UnrealEngine
#include "CoreMinimal.h"
#include "UObject/ScriptMacros.h"
#include "Components/ActorComponent.h"

// Generated
#include "GKLuaComponent.generated.h"

/**
 * Script-extendable component class
 */
UCLASS(Blueprintable, hidecategories = (Object, ActorComponent), editinlinenew, meta = (BlueprintSpawnableComponent), ClassGroup = Script, Abstract)
class GKLUA_API UGKLuaComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:
	/**
	 * Calls a script-defined function (no arguments)
	 * @param FunctionName Name of the function to call
	 */
	UFUNCTION(BlueprintCallable, Category = "Script|Functions")
	virtual bool CallScriptFunction(FString FunctionName);

	//~ Begin UActorComponent Interface.
	virtual void OnRegister() override;
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void OnUnregister() override;
	//~ Begin UActorComponent Interface.

protected:
	/** Script context (code) */
	FScriptContextBase *Context;
};
