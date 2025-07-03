// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Region/RegionActor.h"

// Project Headers
#include "RenCore/Public/Library/MiscLibrary.h"
#include "RenCore/Public/Macro/LogMacro.h"

#include "RenEnvironment/Public/Asset/EnvironmentProfileAsset.h"
#include "RenEnvironment/Public/Subsystem/EnvironmentSubsystem.h"



void ARegionActor::BeginPlay()
{
	if (UWorld* World = GetWorld())
	{
		EnvironmentSubsystem = World->GetSubsystem<UEnvironmentSubsystem>();
	}
	Super::BeginPlay();
}

void ARegionActor::HandlePlayerEntered()
{
	if(!IsValid(EnvironmentSubsystem))
	{
		LOG_ERROR(LogTemp, TEXT("Invalid Environment Subsystem"));
		return;
	}

	for (auto& Kvp : ProfileAssets)
	{
		EnvironmentSubsystem->AddStackedProfile(Kvp.Value, Kvp.Key);
	}
}

void ARegionActor::HandlePlayerExited()
{
	if (!IsValid(EnvironmentSubsystem))
	{
		LOG_ERROR(LogTemp, TEXT("Invalid Environment Subsystem"));
		return;
	}

	for (auto& Kvp : ProfileAssets)
	{
		if (!IsValid(Kvp.Value))
		{
			continue;
		}
		EnvironmentSubsystem->RemoveStackedProfile(Kvp.Value->ProfileType, Kvp.Key);
	}
}

