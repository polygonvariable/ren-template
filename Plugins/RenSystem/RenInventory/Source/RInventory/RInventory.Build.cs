// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RInventory : ModuleRules
{
	public RInventory(ReadOnlyTargetRules Target) : base(Target)
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
				"GameplayTags",
                "RCoreAscension",
                "RCoreAsset",
				"RCoreAssetManager",
                "RCoreAssetInstance",
                "RCoreEquipment",
                "RCoreStorage",
                "RCoreCommon",
                "RCoreShop",
                "RCoreCraft",
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
                "RCoreFilter",
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

