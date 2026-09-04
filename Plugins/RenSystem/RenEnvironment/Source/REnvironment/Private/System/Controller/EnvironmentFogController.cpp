// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Controller/EnvironmentFogController.h"

// Engine Headers
#include "Components/ExponentialHeightFogComponent.h"

// Project Headers
#include "Core/Type/EnvironmentProfileType.h"
#include "Data/EnvironmentProfileAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


UEnvironmentFogController::UEnvironmentFogController()
{
	ProfileType = EEnvironmentProfileType::Fog;
}

void UEnvironmentFogController::Initialize(AActor* Actor)
{
	Super::Initialize(Actor);

	if (IsValid(Actor))
	{
		ExponentialHeightFogComponent = Actor->GetComponentByClass<UExponentialHeightFogComponent>();
	}
}

void UEnvironmentFogController::Deinitialize()
{
	ExponentialHeightFogComponent.Reset();

	Super::Deinitialize();
}

void UEnvironmentFogController::OnPriorityItemChanged(UObject* Item)
{
	Super::OnPriorityItemChanged(Item);

	UEnvironmentFogProfileAsset* Profile = Cast<UEnvironmentFogProfileAsset>(Item);
	UExponentialHeightFogComponent* ExponentialHeightFog = ExponentialHeightFogComponent.Get();
	if (!IsValid(Profile) || !IsValid(ExponentialHeightFog))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("FogProfile or ExponentialHeightFog is invalid"));
		return;
	}

	CurrentDensity = ExponentialHeightFog->FogDensity;
	TargetDensity = Profile->FogDensity;

	StartTransition();
}

void UEnvironmentFogController::OnTransitionChanged(float Alpha)
{
	UExponentialHeightFogComponent* ExponentialHeightFog = ExponentialHeightFogComponent.Get();
	if (!IsValid(ExponentialHeightFog))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("ExponentialHeightFog is invalid"));
		return;
	}
	
	float NewDensity = FMath::Lerp(CurrentDensity, TargetDensity, Alpha);
	ExponentialHeightFog->SetFogDensity(NewDensity);

	PRINT_INFO(LogEnvironment, 5.0f, TEXT("Alpha: %f"), Alpha);
}

