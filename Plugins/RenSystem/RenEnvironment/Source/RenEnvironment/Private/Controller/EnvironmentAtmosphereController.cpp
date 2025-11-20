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

void UEnvironmentAtmosphereController::HandleItemChanged(UObject* Item)
{
	Super::HandleItemChanged(Item);

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

	CurentMieScattering = SkyAtmosphere->MieScatteringScale;
	TargetMieScattering = AtmosphereProfile->MieScatteringScale;

	StartTransition();
}

void UEnvironmentAtmosphereController::HandleTimerTick(float ElapsedTime)
{
	USkyAtmosphereComponent* SkyAtmosphere = SkyAtmosphereComponent.Get();
	if (!IsValid(SkyAtmosphere))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("AtmosphereComponent is invalid"));
		return;
	}

	float Duration = GetTransitionDuration();
	float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);
	float NewMieScattering = FMath::Lerp(CurentMieScattering, TargetMieScattering, Alpha);

	SkyAtmosphere->SetMieScatteringScale(NewMieScattering);

	PRINT_INFO(LogEnvironment, 5.0f, TEXT("Elapsed: %f, Duration: %f, Alpha: %f"), ElapsedTime, Duration, Alpha);
}

