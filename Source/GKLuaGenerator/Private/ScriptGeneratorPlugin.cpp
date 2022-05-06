// BSD 3-Clause License Copyright (c) 2022, Pierre Delaunay All rights reserved.

// GKLua
#include "ScriptGeneratorLog.h"
#include "ScriptCodeGeneratorBase.h"
#include "LuaScriptCodeGenerator.h"

// UnrealEngine
#include "CoreMinimal.h"
#include "Interfaces/IProjectManager.h"
#include "Features/IModularFeatures.h"
#include "Templates/UniquePtr.h"
#include "Misc/ConfigCacheIni.h"
#include "ProjectDescriptor.h"

DEFINE_LOG_CATEGORY(LogScriptGenerator);

class FScriptGeneratorPlugin : public IScriptGeneratorPlugin
{
	/** Specialized script code generator */
	TUniquePtr<FScriptCodeGeneratorBase> CodeGenerator;

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** IScriptGeneratorPlugin interface */
	virtual FString GetGeneratedCodeModuleName() const override { return TEXT("GKLua"); }
	virtual bool ShouldExportClassesForModule(const FString &ModuleName, EBuildModuleType::Type ModuleType, const FString &ModuleGeneratedIncludeDirectory) const override;
	virtual bool SupportsTarget(const FString &TargetName) const override;
	virtual void Initialize(const FString &RootLocalPath, const FString &RootBuildPath, const FString &OutputDirectory, const FString &IncludeBase) override;
	virtual void ExportClass(UClass *Class, const FString &SourceHeaderFilename, const FString &GeneratedHeaderFilename, bool bHasChanged) override;
	virtual void FinishExport() override;
	virtual FString GetGeneratorName() const override;
};

IMPLEMENT_MODULE(FScriptGeneratorPlugin, ScriptGeneratorPlugin)

void FScriptGeneratorPlugin::StartupModule()
{
	// Register ourselves as an editor feature
	IModularFeatures::Get().RegisterModularFeature(TEXT("LuaGenerator"), this);
}

void FScriptGeneratorPlugin::ShutdownModule()
{
	CodeGenerator.Reset();

	// Unregister our feature
	IModularFeatures::Get().UnregisterModularFeature(TEXT("LuaGenerator"), this);
}

FString FScriptGeneratorPlugin::GetGeneratorName() const
{
	return TEXT("GKLuaGenerator");
}

void FScriptGeneratorPlugin::Initialize(const FString &RootLocalPath, const FString &RootBuildPath, const FString &OutputDirectory, const FString &IncludeBase)
{
	UE_LOG(LogScriptGenerator, Log, TEXT("Using Lua Script Generator."));
	CodeGenerator = MakeUnique<FLuaScriptCodeGenerator>(RootLocalPath, RootBuildPath, OutputDirectory, IncludeBase);
}

bool FScriptGeneratorPlugin::ShouldExportClassesForModule(const FString &ModuleName, EBuildModuleType::Type ModuleType, const FString &ModuleGeneratedIncludeDirectory) const
{
	bool bCanExport = (ModuleType == EBuildModuleType::EngineRuntime || ModuleType == EBuildModuleType::GameRuntime);
	if (bCanExport)
	{
		// Only export functions from selected modules
		static struct FSupportedModules
		{
			TArray<FString> SupportedScriptModules;
			FSupportedModules()
			{
				GConfig->GetArray(TEXT("Plugins"), TEXT("ScriptSupportedModules"), SupportedScriptModules, GEngineIni);
			}
		} SupportedModules;
		bCanExport = SupportedModules.SupportedScriptModules.Num() == 0 || SupportedModules.SupportedScriptModules.Contains(ModuleName);
	}
	return bCanExport;
}

void FScriptGeneratorPlugin::ExportClass(UClass *Class, const FString &SourceHeaderFilename, const FString &GeneratedHeaderFilename, bool bHasChanged)
{
	CodeGenerator->ExportClass(Class, SourceHeaderFilename, GeneratedHeaderFilename, bHasChanged);
}

void FScriptGeneratorPlugin::FinishExport()
{
	CodeGenerator->FinishExport();
}

bool FScriptGeneratorPlugin::SupportsTarget(const FString &TargetName) const
{
	// We only support the target if it explicitly enables the required script plugin in its uproject file
	bool bSupportsTarget = false;
	if (FPaths::IsProjectFilePathSet())
	{
		FProjectDescriptor ProjectDescriptor;
		FText OutError;
		if (ProjectDescriptor.Load(FPaths::GetProjectFilePath(), OutError))
		{
			for (auto &PluginDescriptor : ProjectDescriptor.Plugins)
			{
				// For your own script plugin you might want to change the hardcoded name here to something else
				if (PluginDescriptor.bEnabled && PluginDescriptor.Name == TEXT("GKLua"))
				{
					bSupportsTarget = true;
					break;
				}
			}
		}
	}
	return bSupportsTarget;
}
