// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RAvatar : ModuleRules
{
	public RAvatar(ReadOnlyTargetRules Target) : base(Target)
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
                "GameplayAbilities",
                "DeveloperSettings",
                "GameplayTags",
                "CAsset",
                "RCoreCommon",
                "RCoreAscension",
                "CGamedataStorage",
                "RCharacter",
                "CAssetInstance",
                "CAssetManager",
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
                "EnhancedInput",
                "RCoreLibrary",
                "RCoreDelegate",
                "RCoreFilter",
                "CGameplayBase",
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

