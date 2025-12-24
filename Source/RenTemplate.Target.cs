// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class RenTemplateTarget : TargetRules
{
	public RenTemplateTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		/**
		 * After adding new modules make sure to add them here,
		 * and regenerate the project.
		*/

		ExtraModuleNames.AddRange(new string[] { "RenTemplate" });
		ExtraModuleNames.AddRange(
			new string[]
			{
				
			}
		);

	}
}
