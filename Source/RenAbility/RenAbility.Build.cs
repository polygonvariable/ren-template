// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class RenAbility : ModuleRules
{
	public RenAbility(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		/*
		 * This is the top level module for the project.
		 * And should not be included in any other custom modules.
		 * Only engine modules should be used here.
		*/

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"StructUtils",
                "UMG",
            }
		);
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "RenCore",
				"RenAsset",
                "RenStorage",
                "RenGameplay",
            }
        );

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"GameplayAbilities",
				"GameplayTags",
				"GameplayTasks"
            }
		);

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
