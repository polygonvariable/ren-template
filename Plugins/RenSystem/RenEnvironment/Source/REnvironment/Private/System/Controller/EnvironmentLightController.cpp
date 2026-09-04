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

void UEnvironmentLightController::OnPriorityItemChanged(UObject* Item)
{
	Super::OnPriorityItemChanged(Item);

	UDirectionalLightComponent* Sun = SunComponent.Get();
	UDirectionalLightComponent* Moon = MoonComponent.Get();
	UEnvironmentLightProfileAsset* Profile = Cast<UEnvironmentLightProfileAsset>(Item);

	if (!IsValid(Sun) || !IsValid(Moon) || !IsValid(Profile))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("Sun, Moon, Profile is invalid"));
		return;
	}

	CurrentSunIntensity = Sun->Intensity;
	CurrentMoonIntensity = Moon->Intensity;

	CurrentSunColor = Sun->LightColor;
	CurrentMoonColor = Moon->LightColor;

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

	float NewSunIntensity = FMath::Lerp(CurrentSunIntensity, TargetSunIntensity, Alpha);
	float NewMoonIntensity = FMath::Lerp(CurrentMoonIntensity, TargetMoonIntensity, Alpha);

	FVector NewSunColor = FMath::Lerp(FVector(CurrentSunColor), FVector(TargetSunColor), Alpha);
	FVector NewMoonColor = FMath::Lerp(FVector(CurrentMoonColor), FVector(TargetMoonColor), Alpha);

	Sun->Intensity = NewSunIntensity;
	Moon->Intensity = NewMoonIntensity;

	Sun->LightColor = FLinearColor(NewSunColor).ToFColor(false);
	Moon->LightColor = FLinearColor(NewSunColor).ToFColor(false);

	PRINT_INFO(LogEnvironment, 5.0f, TEXT("Alpha: %f"), Alpha);
}

