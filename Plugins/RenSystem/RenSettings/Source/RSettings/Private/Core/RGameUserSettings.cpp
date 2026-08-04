// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/RGameUserSettings.h"

// Project Headers
#include "Log/LogMacro.h"


void URGameUserSettings::LoadSettings(bool bForceReload)
{
	LOG_WARNING(LogTemp, TEXT("Settings loaded"));

	IConsoleManager& ConsoleManager = IConsoleManager::Get();

	RenderCVar = ConsoleManager.FindConsoleVariable(TEXT("r.ScreenPercentage"));
	AACVar = ConsoleManager.FindConsoleVariable(TEXT("r.AntiAliasingMethod"));
	SharpenCVar = ConsoleManager.FindConsoleVariable(TEXT("r.Tonemapper.Sharpen"));
	MaxFPSCVar = ConsoleManager.FindConsoleVariable(TEXT("t.MaxFPS"));

	Super::LoadSettings(bForceReload);
}

void URGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	LOG_WARNING(LogTemp, TEXT("Settings applied"));

	UpdateCVarValues();
	Super::ApplySettings(bCheckForCommandLineOverrides);
}

void URGameUserSettings::SaveSettings()
{
	LOG_WARNING(LogTemp, TEXT("Settings saved"));

	UpdateSettingValues();
	Super::SaveSettings();
}


void URGameUserSettings::RegisterCVar()
{
	BindCVarDelegate(RenderCVar);
	BindCVarDelegate(AACVar);
	BindCVarDelegate(SharpenCVar);
	BindCVarDelegate(MaxFPSCVar);
}

void URGameUserSettings::UnregisterCVar()
{
	ClearCVarDelegate(RenderCVar);
	ClearCVarDelegate(AACVar);
	ClearCVarDelegate(SharpenCVar);
	ClearCVarDelegate(MaxFPSCVar);
}


void URGameUserSettings::BindCVarDelegate(IConsoleVariable* Variable)
{
	if (Variable)
	{
		FConsoleVariableMulticastDelegate& Delegate = Variable->OnChangedDelegate();
		if (!Delegate.IsBoundToObject(this))
		{
			Delegate.AddUObject(this, &URGameUserSettings::HandleOnCVarChanged);
		}
	}
}

void URGameUserSettings::ClearCVarDelegate(IConsoleVariable* Variable)
{
	if (Variable)
	{
		Variable->OnChangedDelegate().RemoveAll(this);
	}
}


void URGameUserSettings::UpdateCVarValues()
{
	UpdateCVarValue(RenderCVar, RenderResolution, 50, 150);
	UpdateCVarValue(AACVar, AntiAliasingMethod, 0, 5);
	UpdateCVarValue(SharpenCVar, TonemapperSharpen, 0, 2);
	UpdateCVarValue(MaxFPSCVar, FrameRateLimit, 0, 120);
}

void URGameUserSettings::UpdateSettingValues()
{
	UpdateSettingValue(RenderCVar, RenderResolution, 50, 150);
	UpdateSettingValue(AACVar, AntiAliasingMethod, 0, 5);
	UpdateSettingValue(SharpenCVar, TonemapperSharpen, 0, 2);
	UpdateSettingValue(MaxFPSCVar, FrameRateLimit, 0, 120);
}


void URGameUserSettings::UpdateCVarValue(IConsoleVariable* Variable, int Value, int Min, int Max)
{
	if (Variable)
	{
		Variable->Set(FMath::Clamp(Value, Min, Max), ECVF_SetByConsole);
	}
}

void URGameUserSettings::UpdateSettingValue(IConsoleVariable* Variable, int& Value, int Min, int Max)
{
	if (Variable)
	{
		Value = FMath::Clamp(Variable->GetInt(), Min, Max);
	}
}

void URGameUserSettings::UpdateSettingValue(IConsoleVariable* Variable, float& Value, int Min, int Max)
{
	if (Variable)
	{
		Value = FMath::Clamp(Variable->GetFloat(), Min, Max);
	}
}


void URGameUserSettings::HandleOnCVarChanged(IConsoleVariable* Variable)
{
	LOG_WARNING(LogTemp, TEXT("Settings updated"));

	UpdateSettingValues();
}

