// BSD 3-Clause License Copyright (c) 2022, Pierre Delaunay All rights reserved.

// GKLua
#include "IGKLua.h"
#include "GKLuaLog.h"
#include "ScriptObjectReferencer.h"

// Unreal
#include "CoreMinimal.h"
#include "UObject/UnrealType.h"

FProperty *FindScriptPropertyHelper(UClass *Class, FName PropertyName)
{
	for (TFieldIterator<FProperty> PropertyIt(Class, EFieldIteratorFlags::ExcludeSuper); PropertyIt; ++PropertyIt)
	{
		FProperty *Property = *PropertyIt;
		if (Property->GetFName() == PropertyName)
		{
			return Property;
		}
	}
	return NULL;
}

DEFINE_LOG_CATEGORY(LogGKLua);

class FGKLua : public IGKLua
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE(FGKLua, GKLua)

void FGKLua::StartupModule()
{
	FScriptObjectReferencer::Init();
}

void FGKLua::ShutdownModule()
{
	FScriptObjectReferencer::Shutdown();
}
