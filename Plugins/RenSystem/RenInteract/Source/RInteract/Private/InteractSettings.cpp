// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InteractSettings.h"

// Project Headers
#include "InteractSubsystem.h"


UInteractSettings::UInteractSettings(const FObjectInitializer& ObjectInitializer)
{
	CategoryName = TEXT("Ren Project");

	SubsystemClass = UInteractSubsystem::StaticClass();
	InteractMode = TEXT("InteractUI");
}

const UInteractSettings* UInteractSettings::Get()
{
	return GetDefault<UInteractSettings>();
}

