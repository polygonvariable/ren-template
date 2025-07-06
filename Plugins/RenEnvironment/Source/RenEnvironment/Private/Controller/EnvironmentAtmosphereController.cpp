// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Controller/EnvironmentAtmosphereController.h"

// Engine Headers
#include "Components/SkyAtmosphereComponent.h"
#include "EngineUtils.h"

// Project Headers
#include "RenCore/Public/Macro/LogMacro.h"

#include "RenEnvironment/Public/Asset/EnvironmentProfileAsset.h"
#include "RenEnvironment/Public/Profile/EnvironmentProfileType.h"



UEnvironmentAtmosphereController::UEnvironmentAtmosphereController()
{
	EnvironmentProfileType = EEnvironmentProfileType::Atmosphere;
}

void UEnvironmentAtmosphereController::InitializeController()
{
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (IsValid(*ActorItr) && ActorItr->ActorHasTag(ActorTag))
		{
			AtmosphereComponent = Cast<USkyAtmosphereComponent>(ActorItr->GetComponentByClass(USkyAtmosphereComponent::StaticClass()));
			break;
		}
	}
}

void UEnvironmentAtmosphereController::CleanupController()
{
	AtmosphereComponent.Reset();
}

void UEnvironmentAtmosphereController::HandleItemChanged(UObject* Item)
{
	if (USkyAtmosphereComponent* AtmosphereComponentPtr = AtmosphereComponent.Get())
	{
		UEnvironmentAtmosphereProfileAsset* AtmosphereProfile = Cast<UEnvironmentAtmosphereProfileAsset>(Item);
		if (IsValid(AtmosphereProfile))
		{
			AtmosphereComponentPtr->SetMieScatteringScale(AtmosphereProfile->MieScatteringScale);
		}
		else
		{
			PRINT_ERROR(LogTemp, 1.0f, TEXT("AtmosphereProfile asset is invalid"));
		}
	}
	else
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("AtmosphereComponent not found"));
	}
}

