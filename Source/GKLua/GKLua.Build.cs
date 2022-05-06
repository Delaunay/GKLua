// BSD 3-Clause License Copyright (c) 2022, Pierre Delaunay All rights reserved.
using System.IO;
using System;
using UnrealBuildTool;

namespace UnrealBuildTool.Rules
{
	public class GKLua : ModuleRules
	{
		public GKLua(ReadOnlyTargetRules Target) : base(Target)
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
					"SlateCore",
					// ... add other public dependencies that you statically link with here ...
				}
				);

			if (Target.bBuildEditor == true)
			{

				PublicDependencyModuleNames.AddRange(
					new string[]
					{
						"EditorFramework",
						"UnrealEd",
					}
				);

			}


			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
					// ... add any modules that your module loads dynamically here ...
				}
				);

			var LuaPath = Path.Combine("..", "Plugins", "GKLua", "Source", "Lua");
			var LuaLibDirectory = Path.Combine(LuaPath, "Lib", Target.Platform.ToString(), "Release");
			var LuaLibPath = Path.Combine(LuaLibDirectory, "Lua.lib");
			if (File.Exists(LuaLibPath))
			{
				PublicDefinitions.Add("WITH_LUA=1");

				// Path to Lua include files
				var IncludePath = Path.GetFullPath(Path.Combine(LuaPath, "Include"));
				PublicIncludePaths.Add(IncludePath);

				// Lib file
				PublicAdditionalLibraries.Add(LuaLibPath);
				Console.WriteLine("LUA Integration enabled:" + IncludePath);
				// Log.TraceVerbose();
			}
			else
			{
				// ("LUA Integration NOT enabled");
				PublicDefinitions.Add("WITH_LUA=0");
			}
		}
	}
}
