// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Controller/EnvironmentAtmosphereController.h"

// Engine Headers
#include "Components/SkyAtmosphereComponent.h"
#include "EngineUtils.h"

// Project Headers
#include "RenCore/Public/Timer/Timer.h"
#include "RenCore/Public/Library/MiscLibrary.h"
#include "RenCore/Public/Macro/LogMacro.h"

#include "RenEnvironment/Public/Asset/EnvironmentProfileAsset.h"
#include "RenEnvironment/Public/Profile/EnvironmentProfileType.h"



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

void UEnvironmentAtmosphereController::HandleItemChanged(UObject* Item)
{
	if (!AtmosphereComponent.IsValid())
	{
		LOG_ERROR(LogTemp, TEXT("AtmosphereComponent not found"));
		return;
	}

	UEnvironmentAtmosphereProfileAsset* AtmosphereProfile = Cast<UEnvironmentAtmosphereProfileAsset>(Item);
	if (!IsValid(AtmosphereProfile))
	{
		PRINT_ERROR(LogTemp, 2.0f, TEXT("AtmosphereProfile asset is invalid"));
		return;
	}

	AtmosphereComponent->SetMieScatteringScale(AtmosphereProfile->MieScatteringScale);
}

