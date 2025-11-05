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

void UEnvironmentLightController::InitializeController(AActor* Actor)
{
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

void UEnvironmentLightController::CleanupController()
{
	SunComponent.Reset();
	MoonComponent.Reset();
}

void UEnvironmentLightController::OnItemChanged(UObject* Item)
{
	UDirectionalLightComponent* Sun = SunComponent.Get();
	UDirectionalLightComponent* Moon = MoonComponent.Get();
	UEnvironmentLightProfileAsset* LightProfile = Cast<UEnvironmentLightProfileAsset>(Item);

	if (!IsValid(Sun) || !IsValid(Moon) || !IsValid(LightProfile))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("Sun, Moon, LightProfile is invalid"));
		return;
	}

	Sun->SetIntensity(LightProfile->SunIntensity);
	//Sun->SetLightColor(LightProfile->SunColor);
	//Moon->SetIntensity(LightProfile->MoonIntensity);
	//Moon->SetLightColor(LightProfile->MoonColor);
}

