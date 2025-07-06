// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Controller/EnvironmentFogController.h"

// Engine Headers
#include "Components/ExponentialHeightFogComponent.h"
#include "EngineUtils.h"

// Project Headers
#include "RenCore/Public/Macro/LogMacro.h"

#include "RenEnvironment/Public/Asset/EnvironmentProfileAsset.h"
#include "RenEnvironment/Public/Profile/EnvironmentProfileType.h"



UEnvironmentFogController::UEnvironmentFogController()
{
	EnvironmentProfileType = EEnvironmentProfileType::Fog;
}

void UEnvironmentFogController::InitializeController()
{
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (IsValid(*ActorItr) && ActorItr->ActorHasTag(ActorTag))
		{
			FogComponent = Cast<UExponentialHeightFogComponent>(ActorItr->GetComponentByClass(UExponentialHeightFogComponent::StaticClass()));
			break;
		}
	}
}

void UEnvironmentFogController::CleanupController()
{
	FogComponent.Reset();
}

void UEnvironmentFogController::HandleItemChanged(UObject* Item)
{
	if (UExponentialHeightFogComponent* FogComponentPtr = FogComponent.Get())
	{
		UEnvironmentFogProfileAsset* FogProfile = Cast<UEnvironmentFogProfileAsset>(Item);
		if (IsValid(FogProfile))
		{
			FogComponentPtr->SetFogDensity(FogProfile->FogDensity);
		}
		else
		{
			PRINT_ERROR(LogTemp, 1.0f, TEXT("FogProfile asset is invalid"));
		}
	}
	else
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("FogComponent not found"));
	}
}

