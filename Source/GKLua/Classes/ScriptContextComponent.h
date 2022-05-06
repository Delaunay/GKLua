// BSD 3-Clause License Copyright (c) 2022, Pierre Delaunay All rights reserved.

#pragma once

// GKLua
#include "LuaBlueprintGeneratedClass.h"

// UnrealEngine
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

// Generated
#include "ScriptContextComponent.generated.h"

/**
 * Script-extendable component class
 */
UCLASS()
class GKLUA_API UScriptContextComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:
	//~ Begin UActorComponent Interface.
	virtual void OnRegister() override;
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void BeginDestroy() override;
	//~ Begin UActorComponent Interface.

	/**
	 * Calls a script-defined function (no arguments)
	 * @param FunctionName Name of the function to call
	 */
	UFUNCTION(BlueprintCallable, Category = "ScriptContext")
	void CallScriptFunction(FString FunctionName);

protected:
	/** Script context (code) */
	FScriptContextBase *Context;
};
