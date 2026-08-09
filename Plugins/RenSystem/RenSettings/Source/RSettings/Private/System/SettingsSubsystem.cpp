// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/SettingsSubsystem.h"

// Project Headers
#include "GameFramework/GameUserSettings.h"
#include "Core/RGameUserSettings.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


bool USettingsSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void USettingsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("SettingsSubsystem initialized"));

	if (IsValid(GEngine))
	{
		URGameUserSettings* Settings = Cast<URGameUserSettings>(GEngine->GetGameUserSettings());
		if (IsValid(Settings))
		{
			Settings->ApplySettings(true);
			Settings->RegisterCVar();
		}
	}
}

void USettingsSubsystem::Deinitialize()
{
	if (IsValid(GEngine))
	{
		URGameUserSettings* Settings = Cast<URGameUserSettings>(GEngine->GetGameUserSettings());
		if (IsValid(Settings))
		{
			Settings->UnregisterCVar();
			Settings->SaveSettings();
		}
	}

	LOG_WARNING(LogTemp, TEXT("SettingsSubsystem deinitialized"));
	Super::Deinitialize();
}

