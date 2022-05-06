// BSD 3-Clause License Copyright (c) 2022, Pierre Delaunay All rights reserved.

namespace UnrealBuildTool.Rules
{
	public class GKLuaEditor : ModuleRules
	{
		public GKLuaEditor(ReadOnlyTargetRules Target) : base(Target)
		{
			PublicIncludePaths.AddRange(
				new string[] {
					"Programs/UnrealHeaderTool/Public",
					// ... add other public include paths required here ...
				}
				);

			PrivateIncludePaths.AddRange(
				new string[] {
					// ... add other private include paths required here ...
				}
				);

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"Engine",
					"InputCore",
					"EditorFramework",
					"UnrealEd",
					"AssetTools",
					"ClassViewer",
					"KismetCompiler",
					"Kismet",
					"BlueprintGraph",

					// Ours
					"GKLua"
					// ... add other public dependencies that you statically link with here ...
				}
				);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					// ... add private dependencies that you statically link with here ...
				}
				);

			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
					// ... add any modules that your module loads dynamically here ...
				}
				);
		}
	}
}
