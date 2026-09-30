// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RCharacterSpawner : ModuleRules
{
	public RCharacterSpawner(ReadOnlyTargetRules Target) : base(Target)
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
				"DeveloperSettings",
                "GameplayAbilities",
                "GameplayTags",
				"CFramework",
                "RCoreAscension",
                "CAsset",
                "CAssetInstance",
                "CAssetManager",
				"CGamedata",
                "CGamedataStorage",
                "CAuthAction",
                "RCharacter",
				"RAvatar",
                "CGamedataComponent",
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
                "CLibrary",
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

