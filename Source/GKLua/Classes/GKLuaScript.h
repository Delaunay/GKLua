// BSD 3-Clause License Copyright (c) 2022, Pierre Delaunay All rights reserved.

#pragma once

// Gamekit
#include "Gamekit/Lua/GKLua.h"

// Unreal Engine
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

// Generated
#include "GKLuaScript.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCodeSourceChanged);


/**
 * Lua script asset, simply hold the path to the lua script.
 * In cooked builds it holds the bytecode
 */
UCLASS()
class GAMEKIT_API UGKLuaScript : public UObject
{
	GENERATED_BODY()

public:

	FString GetScriptPath() const {
		return FPaths::Combine(FPaths::ProjectDir(), LuaScriptPath);
	}

	// Generate the Lua byte code
	void Compile();

	// Cached bytecote
	UPROPERTY()
	TArray<uint8> ByteCote;

	// Asset Management
#if WITH_EDITORONLY_DATA
	// The path is relative to the project's folder
	// use GetScriptPath() to get a path that will resolve to an actual file
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Script)
	FString LuaScriptPath;

	UPROPERTY(Transient)
	FString LuaSourceCode;

	UPROPERTY()
	class UAssetImportData* AssetImportData;
#endif

#if WITH_EDITORONLY_DATA
	/** Override to ensure we write out the asset import data */
	virtual void GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const;
	virtual void PostLoad() override;
#endif

#if WITH_EDITOR
	static bool ValidateGeneratedClass(const UClass* InClass);

	bool IsCodeDirty() const;

	//! Checks if the source code changed and updates it if so (does not recompile the class)
	void UpdateSourceCodeIfChanged();

	FCodeSourceChanged OnCodeSourceChanged;
#endif
};
