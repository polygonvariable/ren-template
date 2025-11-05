// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Controller/EnvironmentAtmosphereController.h"

// Engine Headers
#include "Components/SkyAtmosphereComponent.h"
#include "EngineUtils.h"

// Project Headers
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenEnvironment/Public/Asset/EnvironmentProfileAsset.h"
#include "RenEnvironment/Public/Profile/EnvironmentProfileType.h"



UEnvironmentAtmosphereController::UEnvironmentAtmosphereController()
{
	ProfileType = EEnvironmentProfileType::Atmosphere;
}

void UEnvironmentAtmosphereController::InitializeController(AActor* Actor)
{
	if (IsValid(Actor))
	{
		SkyAtmosphereComponent = Actor->GetComponentByClass<USkyAtmosphereComponent>();
	}
}

void UEnvironmentAtmosphereController::CleanupController()
{
	SkyAtmosphereComponent.Reset();
}

void UEnvironmentAtmosphereController::OnItemChanged(UObject* Item)
{
	USkyAtmosphereComponent* SkyAtmosphere = SkyAtmosphereComponent.Get();
	if (!IsValid(SkyAtmosphere))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("AtmosphereComponent is invalid"));
		return;
	}
	
	UEnvironmentAtmosphereProfileAsset* AtmosphereProfile = Cast<UEnvironmentAtmosphereProfileAsset>(Item);
	if (!IsValid(AtmosphereProfile))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("AtmosphereProfile asset is invalid"));
		return;
	}

	SkyAtmosphere->SetMieScatteringScale(AtmosphereProfile->MieScatteringScale);
}

