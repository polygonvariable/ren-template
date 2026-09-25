// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Controller/EnvironmentLightController.h"

// Engine Headers
#include "Components/DirectionalLightComponent.h"

// Project Headers
#include "Core/Type/EnvironmentProfileType.h"
#include "Data/EnvironmentProfileAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


UEnvironmentLightController::UEnvironmentLightController()
{
	ProfileType = EEnvironmentProfileType::Light;
}

void UEnvironmentLightController::Initialize(AActor* Actor)
{
	Super::Initialize(Actor);

	if (IsValid(Actor))
	{
		TArray<UActorComponent*> SunComponents = Actor->GetComponentsByTag(UDirectionalLightComponent::StaticClass(), SunTag);
		TArray<UActorComponent*> MoonComponents = Actor->GetComponentsByTag(UDirectionalLightComponent::StaticClass(), MoonTag);

		if (SunComponents.IsValidIndex(0) && MoonComponents.IsValidIndex(0))
		{
			SunComponent = Cast<UDirectionalLightComponent>(SunComponents[0]);
			MoonComponent = Cast<UDirectionalLightComponent>(MoonComponents[0]);
		}
	}
}

void UEnvironmentLightController::Deinitialize()
{
	SunComponent.Reset();
	MoonComponent.Reset();

	Super::Deinitialize();
}

void UEnvironmentLightController::OnPriorityItemChanged(UObject* PreviousItem, UObject* NewItem)
{
	Super::OnPriorityItemChanged(PreviousItem, NewItem);

	UDirectionalLightComponent* Sun = SunComponent.Get();
	UDirectionalLightComponent* Moon = MoonComponent.Get();
	UEnvironmentLightProfileAsset* Profile = Cast<UEnvironmentLightProfileAsset>(NewItem);

	if (!IsValid(Sun) || !IsValid(Moon) || !IsValid(Profile))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("Sun, Moon, Profile is invalid"));
		return;
	}

	CurrentSunIntensity = Sun->Intensity;
	CurrentMoonIntensity = Moon->Intensity;

	CurrentSunColor = Sun->LightColor;
	CurrentMoonColor = Moon->LightColor;

	CurrentSunRadius = Sun->LightSourceAngle;
	CurrentMoonRadius = Moon->LightSourceAngle;


	TargetSunRadius = Profile->SunRadius;
	TargetMoonRadius = Profile->MoonRadius;

	TargetSunIntensity = Profile->SunIntensity;
	TargetMoonIntensity = Profile->MoonIntensity;

	TargetSunColor = Profile->SunColor;
	TargetMoonColor = Profile->MoonColor;

	StartTransition();
}

void UEnvironmentLightController::OnTransitionChanged(float Alpha)
{
	UDirectionalLightComponent* Sun = SunComponent.Get();
	UDirectionalLightComponent* Moon = MoonComponent.Get();

	if (!IsValid(Sun) || !IsValid(Moon))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("Sun, Moon is invalid"));
		return;
	}

	Sun->SetLightSourceAngle(FMath::Lerp(CurrentSunRadius, TargetSunRadius, Alpha));
	Moon->SetLightSourceAngle(FMath::Lerp(CurrentMoonRadius, TargetMoonRadius, Alpha));

	Sun->SetIntensity(FMath::Lerp(CurrentSunIntensity, TargetSunIntensity, Alpha));
	Moon->SetIntensity(FMath::Lerp(CurrentMoonIntensity, TargetMoonIntensity, Alpha));

	Sun->SetLightColor(FMath::Lerp(CurrentSunColor, TargetSunColor, Alpha));
	Moon->SetLightColor(FMath::Lerp(CurrentMoonColor, TargetMoonColor, Alpha));
}

