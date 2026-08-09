// Copyright Epic Games, Inc. All Rights Reserved.

using System.Diagnostics;
using System.IO;
using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class RCoreLuau : ModuleRules
{
	public RCoreLuau(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        string LuauPath = Path.Combine(ModuleDirectory, "../ThirdParty/Luau");

        PublicIncludePaths.AddRange(
			new string[] {
                Path.Combine(LuauPath, "Ast/include"),
                Path.Combine(LuauPath, "Bytecode/include"),
                Path.Combine(LuauPath, "CodeGen/include"),
                Path.Combine(LuauPath, "Common/include"),
                Path.Combine(LuauPath, "Compiler/include"),
                Path.Combine(LuauPath, "VM/include"),
            }
			);

		PublicAdditionalLibraries.Add(Path.Combine(LuauPath, "cmake/Release/Luau.Ast.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(LuauPath, "cmake/Release/Luau.Bytecode.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(LuauPath, "cmake/Release/Luau.CodeGen.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(LuauPath, "cmake/Release/Luau.Common.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(LuauPath, "cmake/Release/Luau.Compiler.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(LuauPath, "cmake/Release/Luau.VM.lib"));

        PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
            }
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
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

