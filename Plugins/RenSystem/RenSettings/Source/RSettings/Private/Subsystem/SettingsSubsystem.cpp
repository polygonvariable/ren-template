// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/SettingsSubsystem.h"

// Project Headers



bool USettingsSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void USettingsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USettingsSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

