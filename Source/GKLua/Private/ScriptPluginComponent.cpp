// BSD 3-Clause License Copyright (c) 2022, Pierre Delaunay All rights reserved.

// GKLua
#include "GKLuaComponent.h"

// UnrealEngine
#include "Engine/World.h"

//////////////////////////////////////////////////////////////////////////

UGKLuaComponent::UGKLuaComponent(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = false;
	bAutoActivate = true;
	bWantsInitializeComponent = true;

	Context = nullptr;
}

void UGKLuaComponent::OnRegister()
{
	Super::OnRegister();

	ULuaBlueprintGeneratedClass *ScriptClass = ULuaBlueprintGeneratedClass::GetScriptGeneratedClass(GetClass());
	UWorld *MyWorld = (ScriptClass ? GetWorld() : nullptr);
	if (MyWorld && MyWorld->WorldType != EWorldType::Editor)
	{
		Context = FScriptContextBase::CreateContext(ScriptClass->SourceCode, ScriptClass, this);
		if (!Context || !Context->CanTick())
		{
			bAutoActivate = false;
			PrimaryComponentTick.bCanEverTick = false;
		}
	}
}

void UGKLuaComponent::InitializeComponent()
{
	Super::InitializeComponent();
	if (Context)
	{
		ULuaBlueprintGeneratedClass *ScriptClass = ULuaBlueprintGeneratedClass::GetScriptGeneratedClass(GetClass());
		Context->PushScriptPropertyValues(ScriptClass, this);
		Context->BeginPlay();
		Context->FetchScriptPropertyValues(ScriptClass, this);
	}
}

void UGKLuaComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Context)
	{
		ULuaBlueprintGeneratedClass *ScriptClass = ULuaBlueprintGeneratedClass::GetScriptGeneratedClass(GetClass());
		Context->PushScriptPropertyValues(ScriptClass, this);
		Context->Tick(DeltaTime);
		Context->FetchScriptPropertyValues(ScriptClass, this);
	}
};

void UGKLuaComponent::OnUnregister()
{
	if (Context)
	{
		Context->Destroy();
		delete Context;
		Context = nullptr;
	}

	Super::OnUnregister();
}

bool UGKLuaComponent::CallScriptFunction(FString FunctionName)
{
	bool bSuccess = false;
	if (Context)
	{
		ULuaBlueprintGeneratedClass *ScriptClass = ULuaBlueprintGeneratedClass::GetScriptGeneratedClass(GetClass());
		Context->PushScriptPropertyValues(ScriptClass, this);
		bSuccess = Context->CallFunction(FunctionName);
		Context->FetchScriptPropertyValues(ScriptClass, this);
	}
	return bSuccess;
}
