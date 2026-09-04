// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/EnvironmentSettings.h"


UEnvironmentSettings::UEnvironmentSettings(const FObjectInitializer& ObjectInitializer)
{
	CategoryName = TEXT("Ren Project");

	EnvironmentBundles.Add(TEXT("Environment"));
}

const UEnvironmentSettings* UEnvironmentSettings::Get()
{
	return GetDefault<UEnvironmentSettings>();
}

