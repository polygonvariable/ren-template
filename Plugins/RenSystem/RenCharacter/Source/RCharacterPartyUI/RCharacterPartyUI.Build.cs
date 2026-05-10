// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RCharacterPartyUI : ModuleRules
{
	public RCharacterPartyUI(ReadOnlyTargetRules Target) : base(Target)
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
                "UMG",
                "RCoreAssetUI",
                "RCharacter",
                "RAvatar",
                "RAvatarUI",
                "RCharacterParty",
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
                "RCoreLibrary",
                "RCoreFilter",
                "RCoreAsset",
                "RCoreDelegate",
                "RCoreAscensionUI",
                "RCoreAssetManager",
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

