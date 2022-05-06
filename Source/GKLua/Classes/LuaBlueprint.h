// BSD 3-Clause License Copyright (c) 2022, Pierre Delaunay All rights reserved.

#pragma once

// GKLua
#include "LuaBlueprintGeneratedClass.h"

// UnrealEngine
#include "Engine/Blueprint.h"

// Generated
#include "LuaBlueprint.generated.h"

/**
 * The Script blueprint generates script-defined classes
 */
UCLASS(BlueprintType)
class GKLUA_API ULuaBlueprint : public UBlueprint
{
	GENERATED_UCLASS_BODY()
public:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Script)
	FString SourceFilePath;

	UPROPERTY()
	class UAssetImportData *AssetImportData;
#endif

public:
	/** Generated script bytecode */
	UPROPERTY()
	TArray<uint8> ByteCode;

	/** Script source code. @todo: this should be editor-only */
	UPROPERTY()
	FString SourceCode;

#if WITH_EDITORONLY_DATA
	/** Override to ensure we write out the asset import data */
	virtual void GetAssetRegistryTags(TArray<FAssetRegistryTag> &OutTags) const;
	virtual void PostLoad() override;
#endif

#if WITH_EDITOR
	virtual UClass *GetBlueprintClass() const override;

	virtual bool SupportedByDefaultBlueprintFactory() const override
	{
		return false;
	}
	// End of UBlueprint interface

	static bool ValidateGeneratedClass(const UClass *InClass);

	bool IsCodeDirty() const;
	void UpdateScriptStatus();

	/**
	 * Checks if the source code changed and updates it if so (does not recompile the class)
	 */
	void UpdateSourceCodeIfChanged();
#endif
};