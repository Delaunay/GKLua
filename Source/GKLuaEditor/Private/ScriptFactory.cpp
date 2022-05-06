// BSD 3-Clause License Copyright (c) 2022, Pierre Delaunay All rights reserved.

// GKLua
#include "ScriptFactory.h"
#include "ScriptEditorPluginLog.h"
#include "ReimportScriptFactory.h"
#include "LuaBlueprintGeneratedClass.h"

// UnrealEngine
#include "GameFramework/Actor.h"
#include "Modules/ModuleManager.h"
#include "ClassViewerModule.h"
#include "Kismet2/SClassPickerDialog.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Editor.h"
#include "UnrealEdGlobals.h"
#include "EditorFramework/AssetImportData.h"
#include "HAL/FileManager.h"
#include "Templates/UniquePtr.h"

#define LOCTEXT_NAMESPACE "EditorScriptFactory"

UScriptFactory::UScriptFactory(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = ULuaBlueprint::StaticClass();
	ParentClass = AActor::StaticClass();

	FScriptContextBase::GetSupportedScriptFileFormats(Formats);

	bCreateNew = false;
	bEditorImport = true;
	bText = true;
	bEditAfterNew = true;
}

bool UScriptFactory::ConfigureProperties()
{
	// Null the parent class so we can check for selection later
	ParentClass = nullptr;

	// Load the class viewer module to display a class picker
	FClassViewerModule &ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");

	// Fill in options
	FClassViewerInitializationOptions Options;
	Options.Mode = EClassViewerMode::ClassPicker;
	Options.DisplayMode = EClassViewerDisplayMode::TreeView;
	Options.bShowObjectRootClass = true;
	Options.bIsBlueprintBaseOnly = true;
	Options.bShowUnloadedBlueprints = true;
	Options.bEnableClassDynamicLoading = true;
	Options.NameTypeToDisplay = EClassViewerNameTypeToDisplay::Dynamic;

	/*
	// Prevent creating blueprints of classes that require special setup (they'll be allowed in the corresponding factories / via other means)
	TSharedPtr<FBlueprintParentFilter> Filter = MakeShareable(new FBlueprintParentFilter);
	Options.ClassFilters.Add(Filter.ToSharedRef());
	if (!IsMacroFactory())
	{
		Filter->DisallowedChildrenOfClasses.Add(ALevelScriptActor::StaticClass());
		Filter->DisallowedChildrenOfClasses.Add(UAnimInstance::StaticClass());
	}

	// Filter out interfaces in all cases; they can never contain code, so it doesn't make sense to use them as a macro basis
	Filter->DisallowedChildrenOfClasses.Add(UInterface::StaticClass());
	*/

	const FText TitleText = NSLOCTEXT("EditorFactories", "CreateScriptOptions", "Pick Parent Class");
	UClass *ChosenClass = nullptr;
	const bool bPressedOk = SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, ULuaBlueprintGeneratedClass::StaticClass());
	if (bPressedOk)
	{
		ParentClass = ChosenClass;
	}

	return bPressedOk;
}

UObject *UScriptFactory::FactoryCreateNew(UClass *Class, UObject *InParent, FName Name, EObjectFlags Flags, UObject *Context, FFeedbackContext *Warn, FName CallingContext)
{
	// Make sure we are trying to factory a blueprint, then create and init one
	check(Class->IsChildOf(UBlueprint::StaticClass()));

	if ((ParentClass == nullptr) || !FKismetEditorUtilities::CanCreateBlueprintOfClass(ParentClass))
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("ClassName"), (ParentClass != nullptr) ? FText::FromString(ParentClass->GetName()) : LOCTEXT("Null", "(null)"));
		FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("CannotCreateBlueprintFromClass", "Cannot create a blueprint based on the class '{0}'."), Args));
		return nullptr;
	}
	else
	{

		GEditor->SelectNone(true, true, false);

		ULuaBlueprint *NewBlueprint = nullptr;
		TUniquePtr<FScriptContextBase> ScriptContext(FScriptContextBase::CreateContext("", nullptr, nullptr));

		if (ScriptContext)
		{
			NewBlueprint = Cast<ULuaBlueprint>(FindObject<UBlueprint>(InParent, *Name.ToString()));
			if (NewBlueprint != nullptr)
			{
				NewBlueprint->Modify();
			}
			else
			{
				NewBlueprint = CastChecked<ULuaBlueprint>(FKismetEditorUtilities::CreateBlueprint(
					ParentClass,
					InParent,
					Name,
					BPTYPE_Normal,
					ULuaBlueprint::StaticClass(),
					ULuaBlueprintGeneratedClass::StaticClass(),
					"UScriptFactory"));
			}

			NewBlueprint->SourceCode = "";
			NewBlueprint->AssetImportData->Update(CurrentFilename);

			// Need to make sure we compile with the new source code
			FKismetEditorUtilities::CompileBlueprint(NewBlueprint);
			GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPostImport(this, NewBlueprint);
		}
		else
		{
			UE_LOG(LogGKLuaEditor, Warning, TEXT("Failed to import %s: could not compile script."), *CurrentFilename);
		}

		return NewBlueprint;
	}
}

UObject *UScriptFactory::FactoryCreateNew(UClass *Class, UObject *InParent, FName Name, EObjectFlags Flags, UObject *Context, FFeedbackContext *Warn)
{
	return FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn, NAME_None);
}

/*
bool UScriptFactory::DoesSupportClass(UClass* Class)
{
	return Class == ULuaBlueprint::StaticClass();
} */

UObject *UScriptFactory::FactoryCreateText(UClass *InClass, UObject *InParent, FName InName, EObjectFlags Flags, UObject *Context, const TCHAR *Type, const TCHAR *&Buffer, const TCHAR *BufferEnd, FFeedbackContext *Warn)
{
	GEditor->SelectNone(true, true, false);

	ULuaBlueprint *NewBlueprint = nullptr;
	TUniquePtr<FScriptContextBase> ScriptContext(FScriptContextBase::CreateContext(Buffer, nullptr, nullptr));
	if (ScriptContext)
	{
		NewBlueprint = Cast<ULuaBlueprint>(FindObject<UBlueprint>(InParent, *InName.ToString()));
		if (NewBlueprint != nullptr)
		{
			NewBlueprint->Modify();
		}
		else
		{
			NewBlueprint = CastChecked<ULuaBlueprint>(FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent, InName, BPTYPE_Normal, ULuaBlueprint::StaticClass(), ULuaBlueprintGeneratedClass::StaticClass(), "UScriptFactory"));
		}

		NewBlueprint->SourceCode = Buffer;

		NewBlueprint->AssetImportData->Update(CurrentFilename);

		// Need to make sure we compile with the new source code
		FKismetEditorUtilities::CompileBlueprint(NewBlueprint);

		GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPostImport(this, NewBlueprint);
	}
	else
	{
		UE_LOG(LogGKLuaEditor, Warning, TEXT("Failed to import %s: could not compile script."), *CurrentFilename);
	}

	return NewBlueprint;
}

/** UReimportScriptFactory */

UReimportScriptFactory::UReimportScriptFactory(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UReimportScriptFactory::ConfigureProperties()
{
	return UFactory::ConfigureProperties();
}

bool UReimportScriptFactory::CanReimport(UObject *Obj, TArray<FString> &OutFilenames)
{
	ULuaBlueprint *ScriptClass = Cast<ULuaBlueprint>(Obj);
	if (ScriptClass)
	{
		ScriptClass->AssetImportData->ExtractFilenames(OutFilenames);
		return true;
	}
	return false;
}

void UReimportScriptFactory::SetReimportPaths(UObject *Obj, const TArray<FString> &NewReimportPaths)
{
	ULuaBlueprint *ScriptClass = Cast<ULuaBlueprint>(Obj);
	if (ScriptClass && ensure(NewReimportPaths.Num() == 1))
	{
		ScriptClass->AssetImportData->UpdateFilenameOnly(NewReimportPaths[0]);
	}
}

/**
 * Reimports specified texture from its source material, if the meta-data exists
 */
EReimportResult::Type UReimportScriptFactory::Reimport(UObject *Obj)
{
	ULuaBlueprint *ScriptClass = Cast<ULuaBlueprint>(Obj);
	if (!ScriptClass)
	{
		return EReimportResult::Failed;
	}

	TGuardValue<ULuaBlueprint *> OriginalScriptGuardValue(OriginalScript, ScriptClass);

	const FString ResolvedSourceFilePath = ScriptClass->AssetImportData->GetFirstFilename();
	if (!ResolvedSourceFilePath.Len())
	{
		return EReimportResult::Failed;
	}

	UE_LOG(LogGKLuaEditor, Log, TEXT("Performing atomic reimport of [%s]"), *ResolvedSourceFilePath);

	// Ensure that the file provided by the path exists
	if (IFileManager::Get().FileSize(*ResolvedSourceFilePath) == INDEX_NONE)
	{
		UE_LOG(LogGKLuaEditor, Warning, TEXT("Cannot reimport: source file cannot be found."));
		return EReimportResult::Failed;
	}

	bool OutCanceled = false;

	if (ImportObject(ScriptClass->GetClass(), ScriptClass->GetOuter(), *ScriptClass->GetName(), RF_Public | RF_Standalone, ResolvedSourceFilePath, nullptr, OutCanceled) != nullptr)
	{
		UE_LOG(LogGKLuaEditor, Log, TEXT("Imported successfully"));
		// Try to find the outer package so we can dirty it up
		if (ScriptClass->GetOuter())
		{
			ScriptClass->GetOuter()->MarkPackageDirty();
		}
		else
		{
			ScriptClass->MarkPackageDirty();
		}
	}
	else if (OutCanceled)
	{
		UE_LOG(LogGKLuaEditor, Warning, TEXT("-- import canceled"));
	}
	else
	{
		UE_LOG(LogGKLuaEditor, Warning, TEXT("-- import failed"));
	}

	return EReimportResult::Succeeded;
}

int32 UReimportScriptFactory::GetPriority() const
{
	return ImportPriority;
}
