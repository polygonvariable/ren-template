// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class REnemyPartyEd : ModuleRules
{
	public REnemyPartyEd(ReadOnlyTargetRules Target) : base(Target)
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
				"UnrealEd",
                "Slate",
                "SlateCore",
                "EditorFramework",
                "DeveloperSettings",
                "StructUtils",
                "GameplayAbilities",
                "GameplayTags",
				"RCoreCommon",
                "RCoreAscension",
                "RCoreAsset",
                "RCoreAssetInstance",
                "RCoreAssetManager",
                "RCoreStorage",
                "RCoreAuthAction",
                "RCharacter",
				"RAvatar",
                "REnemy",
                "REnemyParty",
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
                "RCoreDelegate",
                "RCoreLibrary",
                "StructUtils",
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

