// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Controller/EnvironmentFogController.h"

// Engine Headers
#include "Components/ExponentialHeightFogComponent.h"
#include "EngineUtils.h"

// Project Headers
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenEnvironment/Public/Asset/EnvironmentProfileAsset.h"
#include "RenEnvironment/Public/Profile/EnvironmentProfileType.h"



UEnvironmentFogController::UEnvironmentFogController()
{
	ProfileType = EEnvironmentProfileType::Fog;
}

void UEnvironmentFogController::InitializeController(AActor* Actor)
{
	if (IsValid(Actor))
	{
		ExponentialHeightFogComponent = Actor->GetComponentByClass<UExponentialHeightFogComponent>();
	}
}

void UEnvironmentFogController::CleanupController()
{
	ExponentialHeightFogComponent.Reset();
}

void UEnvironmentFogController::OnItemChanged(UObject* Item)
{
	UExponentialHeightFogComponent* ExponentialHeightFog = ExponentialHeightFogComponent.Get();
	if (!IsValid(ExponentialHeightFog))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("ExponentialHeightFog is invalid"));
		return;
	}

	UEnvironmentFogProfileAsset* FogProfile = Cast<UEnvironmentFogProfileAsset>(Item);
	if (!IsValid(FogProfile))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("FogProfile asset is invalid"));
		return;
	}

	ExponentialHeightFog->SetFogDensity(FogProfile->FogDensity);
}

