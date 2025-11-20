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

void UEnvironmentFogController::HandleItemChanged(UObject* Item)
{
	Super::HandleItemChanged(Item);

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

	CurentDensity = ExponentialHeightFog->FogDensity;
	TargetDensity = FogProfile->FogDensity;

	StartTransition();
}

void UEnvironmentFogController::HandleTimerTick(float ElapsedTime)
{
	UExponentialHeightFogComponent* ExponentialHeightFog = ExponentialHeightFogComponent.Get();
	if (!IsValid(ExponentialHeightFog))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("ExponentialHeightFog is invalid"));
		return;
	}

	float Duration = GetTransitionDuration();
	float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);
	float NewDensity = FMath::Lerp(CurentDensity, TargetDensity, Alpha);

	ExponentialHeightFog->SetFogDensity(NewDensity);

	PRINT_INFO(LogEnvironment, 5.0f, TEXT("Elapsed: %f, Duration: %f, Alpha: %f"), ElapsedTime, Duration, Alpha);
}

