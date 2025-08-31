// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RenDialogueEd : ModuleRules
{
	public RenDialogueEd(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
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
				"AssetTools",
                "UnrealEd",
                "PropertyEditor",
                "EditorScriptingUtilities",
                "ToolMenus",
                "GraphEditor",

                "BlueprintGraph",
                "Kismet",
                "KismetCompiler",
                "MainFrame",
                "EditorFramework",
                "WorkspaceMenuStructure",
                "EditorWidgets",
                "SourceControl",
                "ApplicationCore",

                "RenEventflow",
                "RenEventflowEd",
                "RenDialogue",
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

