// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RCharacterParty : ModuleRules
{
	public RCharacterParty(ReadOnlyTargetRules Target) : base(Target)
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
                "StructUtils",
                "GameplayAbilities",
                "GameplayTags",
                "RCoreAscension",
                "RCoreAsset",
                "RCoreAssetInstance",
                "RCoreAssetManager",
                "RCoreStorage",
				"RCharacter",
				"RAvatar",
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

