// BSD 3-Clause License Copyright (c) 2022, Pierre Delaunay All rights reserved.

#pragma once

// GKLua
#include "LuaBlueprint.h"

// UnrealEngine
#include "CoreMinimal.h"
#include "Factories/Factory.h"

// Generated
#include "ScriptFactory.generated.h"

/**
 * Script Blueprint factory
 */
UCLASS(collapsecategories, hidecategories = Object)
class SCRIPTEDITORPLUGIN_API UScriptFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, Category = ScriptFactory, meta = (AllowAbstract = "", BlueprintBaseOnly = ""))
	TSubclassOf<class UObject> ParentClass;

	//~ Begin UFactory Interface
	virtual bool ConfigureProperties() override;

	virtual UObject *FactoryCreateNew(UClass *Class, UObject *InParent, FName Name, EObjectFlags Flags, UObject *Context, FFeedbackContext *Warn, FName CallingContext) override;
	virtual UObject *FactoryCreateNew(UClass *Class, UObject *InParent, FName Name, EObjectFlags Flags, UObject *Context, FFeedbackContext *Warn) override;

	// virtual bool DoesSupportClass(UClass* Class) override;

	virtual UObject *FactoryCreateText(UClass *InClass, UObject *InParent, FName InName, EObjectFlags Flags, UObject *Context, const TCHAR *Type, const TCHAR *&Buffer, const TCHAR *BufferEnd, FFeedbackContext *Warn);
	//~ End UFactory Interface
};
