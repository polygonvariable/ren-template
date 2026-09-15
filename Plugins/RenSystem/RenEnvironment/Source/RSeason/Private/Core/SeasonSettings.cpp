// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/SeasonSettings.h"

// Project Headers
#include "System/SeasonSubsystem.h"


USeasonSettings::USeasonSettings(const FObjectInitializer& ObjectInitializer)
{
	CategoryName = TEXT("Ren Project");

	SubsystemClass = USeasonSubsystem::StaticClass();
}

const USeasonSettings* USeasonSettings::Get()
{
	return GetDefault<USeasonSettings>();
}

