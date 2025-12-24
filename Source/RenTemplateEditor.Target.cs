// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class RenTemplateEditorTarget : TargetRules
{
	public RenTemplateEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

        ExtraModuleNames.AddRange(new string[] { "RenTemplate" });
        ExtraModuleNames.AddRange(
            new string[]
            {
                
            }
        );

    }
}
