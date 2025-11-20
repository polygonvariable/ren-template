// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Controller/EnvironmentLightController.h"

// Engine Headers
#include "Components/DirectionalLightComponent.h"
#include "EngineUtils.h"

// Project Headers
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenEnvironment/Public/Asset/EnvironmentProfileAsset.h"
#include "RenEnvironment/Public/Profile/EnvironmentProfileType.h"



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

void UEnvironmentLightController::HandleItemChanged(UObject* Item)
{
	Super::HandleItemChanged(Item);

	UDirectionalLightComponent* Sun = SunComponent.Get();
	UDirectionalLightComponent* Moon = MoonComponent.Get();
	UEnvironmentLightProfileAsset* LightProfile = Cast<UEnvironmentLightProfileAsset>(Item);

	if (!IsValid(Sun) || !IsValid(Moon) || !IsValid(LightProfile))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("Sun, Moon, LightProfile is invalid"));
		return;
	}

	CurrentSunIntensity = Sun->Intensity;
	CurrentMoonIntensity = Moon->Intensity;

	CurrentSunColor = Sun->LightColor;
	CurrentMoonColor = Moon->LightColor;

	TargetSunIntensity = LightProfile->SunIntensity;
	TargetMoonIntensity = LightProfile->MoonIntensity;

	TargetSunColor = LightProfile->SunColor;
	TargetMoonColor = LightProfile->MoonColor;

	StartTransition();
}

void UEnvironmentLightController::HandleTimerTick(float ElapsedTime)
{
	UDirectionalLightComponent* Sun = SunComponent.Get();
	UDirectionalLightComponent* Moon = MoonComponent.Get();

	if (!IsValid(Sun) || !IsValid(Moon))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("Sun, Moon is invalid"));
		return;
	}

	float Duration = GetTransitionDuration();
	float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);

	float NewSunIntensity = FMath::Lerp(CurrentSunIntensity, TargetSunIntensity, Alpha);
	float NewMoonIntensity = FMath::Lerp(CurrentMoonIntensity, TargetMoonIntensity, Alpha);

	FVector NewSunColor = FMath::Lerp(FVector(CurrentSunColor), FVector(TargetSunColor), Alpha);
	FVector NewMoonColor = FMath::Lerp(FVector(CurrentMoonColor), FVector(TargetMoonColor), Alpha);

	Sun->Intensity = NewSunIntensity;
	Moon->Intensity = NewMoonIntensity;

	Sun->LightColor = FLinearColor(NewSunColor).ToFColor(false);
	Moon->LightColor = FLinearColor(NewSunColor).ToFColor(false);

	PRINT_INFO(LogEnvironment, 5.0f, TEXT("Elapsed: %f, Duration: %f, Alpha: %f"), ElapsedTime, Duration, Alpha);
}

