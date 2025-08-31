// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class RPGTemplateTarget : TargetRules
{
	public RPGTemplateTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

        /**
		 * After adding new modules make sure to add them here,
		 * and regenerate the project.
		*/

        ExtraModuleNames.AddRange(new string[] { "RPGTemplate" });
        ExtraModuleNames.AddRange(
            new string[]
            {
                //"RenGlobal",
                //"RenWidget",
                //"RenCore",
                //"RenAsset",
                //"RenEventflow",
                //"RenDialogue",
                //"RenGameplay",
                "RenAbility",
                //"RenInventory",
                "RenEntity",
                //"RenEnvironment",
                //"RenInteract",
            }
        );

    }
}
