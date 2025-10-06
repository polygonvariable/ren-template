// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RenExchangeItem : ModuleRules
{
	public RenExchangeItem(ReadOnlyTargetRules Target) : base(Target)
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
                "RenInventory",
                "RCoreExchange",
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
                "UMG",
                "StructUtils",
                "RCoreDelegate",
                "RCoreLibrary",
                "RCoreFilter",
                "RCoreShop",
                "RCoreCraft",
				
                "RCorePool",

                "RCoreInventory",
                "RCoreCounter",
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

