// BSD 3-Clause License Copyright (c) 2022, Pierre Delaunay All rights reserved.

#pragma once

// Gamekit
#include "Gamekit/Lua/GKLua.h"

// Unreal Engine
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

// Generated
#include "GKLuaScript.generated.h"

/**
 * Lua script asset, simply hold the path to the lua script.
 * In cooked builds it holds the bytecode
 * 
 */
UCLASS()
class GAMEKIT_API UGKLuaScript : public UObject
{
	GENERATED_BODY()

public:
	// The path is relative to the project's folder
	// use GetScriptPath() to get a path that will resolve to an actual file
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString LuaScriptPath;

	// UPROPERTY(Transient)
	// FString Code;

	FString GetScriptPath() {
		return FPaths::Combine(FPaths::ProjectDir(), LuaScriptPath);
	}

	// Generate the Lua byte code
	void Compile();

	// Cached bytecote
	TArray<uint8> ByteCote;
};