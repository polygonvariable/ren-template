// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "HSettings.h"

#include "Log/LogMacro.h"


void URGameSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	Super::ApplySettings(bCheckForCommandLineOverrides);

	IConsoleManager& ConsoleManager = IConsoleManager::Get();

	LOG_WARNING(LogTemp, TEXT("Settings loaded start"));

	if (IConsoleVariable* RenderCVar = ConsoleManager.FindConsoleVariable(TEXT("r.ScreenPercentage")))
	{
		RenderCVar->Set(FMath::Clamp(RenderResolution, 50, 150), ECVF_SetByConsole);
	}

	if (IConsoleVariable* AACVar = ConsoleManager.FindConsoleVariable(TEXT("r.AntiAliasingMethod")))
	{
		AACVar->Set(FMath::Clamp(AntiAliasingMethod, 0, 5), ECVF_SetByConsole);
	}

	if (IConsoleVariable* SharpenCVar = ConsoleManager.FindConsoleVariable(TEXT("r.Tonemapper.Sharpen")))
	{
		SharpenCVar->Set(FMath::Clamp(TonemapperSharpen, 0, 2), ECVF_SetByConsole);
	}
	
	LOG_WARNING(LogTemp, TEXT("Settings loaded end"));
}
