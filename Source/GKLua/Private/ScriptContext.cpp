// BSD 3-Clause License Copyright (c) 2022, Pierre Delaunay All rights reserved.

#include "ScriptContext.h"
#include "LuaBlueprintGeneratedClass.h"

//////////////////////////////////////////////////////////////////////////

UScriptContext::UScriptContext(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
	Context = NULL;
}

void UScriptContext::PostInitProperties()
{
	Super::PostInitProperties();

	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		auto ContextOwner = GetOuter();
		if (ContextOwner && !ContextOwner->HasAnyFlags(RF_ClassDefaultObject))
		{
			auto ScriptClass = ULuaBlueprintGeneratedClass::GetScriptGeneratedClass(ContextOwner->GetClass());
			if (ScriptClass)
			{
				Context = FScriptContextBase::CreateContext(ScriptClass->SourceCode, ScriptClass, ContextOwner);
			}
		}
	}
}

void UScriptContext::BeginDestroy()
{
	if (Context)
	{
		Context->Destroy();
		delete Context;
		Context = NULL;
	}

	Super::BeginDestroy();
}

void UScriptContext::CallScriptFunction(FString FunctionName)
{
	if (Context)
	{
		auto ContextOwner = GetOuter();
		auto ScriptClass = ULuaBlueprintGeneratedClass::GetScriptGeneratedClass(ContextOwner->GetClass());
		Context->PushScriptPropertyValues(ScriptClass, ContextOwner);
		Context->CallFunction(FunctionName);
		Context->FetchScriptPropertyValues(ScriptClass, ContextOwner);
	}
}