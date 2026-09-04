// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Controller/EnvironmentAtmosphereController.h"

// Engine Headers
#include "Components/SkyAtmosphereComponent.h"

// Project Headers
#include "Core/Type/EnvironmentProfileType.h"
#include "Data/EnvironmentProfileAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


UEnvironmentAtmosphereController::UEnvironmentAtmosphereController()
{
	ProfileType = EEnvironmentProfileType::Atmosphere;
}

void UEnvironmentAtmosphereController::Initialize(AActor* Actor)
{
	Super::Initialize(Actor);

	if (IsValid(Actor))
	{
		SkyAtmosphereComponent = Actor->GetComponentByClass<USkyAtmosphereComponent>();
	}
}

void UEnvironmentAtmosphereController::Deinitialize()
{
	SkyAtmosphereComponent.Reset();

	Super::Deinitialize();
}

void UEnvironmentAtmosphereController::OnPriorityItemChanged(UObject* Item)
{
	Super::OnPriorityItemChanged(Item);

	UEnvironmentAtmosphereProfileAsset* Profile = Cast<UEnvironmentAtmosphereProfileAsset>(Item);
	USkyAtmosphereComponent* SkyAtmosphere = SkyAtmosphereComponent.Get();
	if (!IsValid(Profile)|| !IsValid(SkyAtmosphere))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("AtmosphereProfile or AtmosphereComponent is invalid"));
		return;
	}

	CurrentMieScattering = SkyAtmosphere->MieScatteringScale;
	TargetMieScattering = Profile->MieScatteringScale;

	StartTransition();
}

void UEnvironmentAtmosphereController::OnTransitionChanged(float Alpha)
{
	USkyAtmosphereComponent* SkyAtmosphere = SkyAtmosphereComponent.Get();
	if (!IsValid(SkyAtmosphere))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("AtmosphereComponent is invalid"));
		return;
	}

	float NewMieScattering = FMath::Lerp(CurrentMieScattering, TargetMieScattering, Alpha);

	SkyAtmosphere->SetMieScatteringScale(NewMieScattering);

	PRINT_INFO(LogEnvironment, 5.0f, TEXT("Alpha: %f"), Alpha);
}

