// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/EnvironmentSettings.h"

// Project Headers
#include "System/EnvironmentSubsystem.h"


UEnvironmentSettings::UEnvironmentSettings(const FObjectInitializer& ObjectInitializer)
{
	CategoryName = TEXT("Ren Project");

	SubsystemClass = UEnvironmentSubsystem::StaticClass();
	EnvironmentBundles.Add(TEXT("Environment"));
}

const UEnvironmentSettings* UEnvironmentSettings::Get()
{
	return GetDefault<UEnvironmentSettings>();
}

