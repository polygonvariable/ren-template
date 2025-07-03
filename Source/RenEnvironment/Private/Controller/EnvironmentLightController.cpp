// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Controller/EnvironmentLightController.h"

// Engine Headers
#include "Components/DirectionalLightComponent.h"
#include "EngineUtils.h"

// Project Headers
#include "RenCore/Public/Timer/Timer.h"
#include "RenCore/Public/Macro/LogMacro.h"
#include "RenCore/Public/Library/MiscLibrary.h"

#include "RenEnvironment/Public/Asset/EnvironmentProfileAsset.h"
#include "RenEnvironment/Public/Profile/EnvironmentProfileType.h"



void UEnvironmentLightController::InitializeController()
{
	bool bSunFound = false;
	bool bMoonFound = false;

	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (IsValid(*ActorItr) && ActorItr->ActorHasTag(ActorTag))
		{
			TArray<UActorComponent*> SunComponents = ActorItr->GetComponentsByTag(UDirectionalLightComponent::StaticClass(), SunComponentName);
			if (SunComponents.IsValidIndex(0) && IsValid(SunComponents[0]))
			{
				SunComponent = Cast<UDirectionalLightComponent>(SunComponents[0]);
				bSunFound = true;
			}

			TArray<UActorComponent*> MoonComponents = ActorItr->GetComponentsByTag(UDirectionalLightComponent::StaticClass(), MoonComponentName);
			if (MoonComponents.IsValidIndex(0) && IsValid(MoonComponents[0]))
			{
				MoonComponent = Cast<UDirectionalLightComponent>(MoonComponents[0]);
				bMoonFound = true;
			}

			break;
		}
	}

	if (!bSunFound) LOG_ERROR(LogTemp, TEXT("Sun not found"));
	if (!bMoonFound) LOG_ERROR(LogTemp, TEXT("Moon not found"));
}

void UEnvironmentLightController::HandleItemChanged(UObject* Item)
{
	if (!SunComponent.IsValid() || !MoonComponent.IsValid())
	{
		LOG_ERROR(LogTemp, TEXT("Sun or Moon not found"));
		return;
	}

	UEnvironmentLightProfileAsset* LightProfile = Cast<UEnvironmentLightProfileAsset>(Item);
	if (!IsValid(LightProfile))
	{
		PRINT_ERROR(LogTemp, 2.0f, TEXT("LightProfile asset is invalid"));
		return;
	}

	SunComponent->SetIntensity(LightProfile->SunIntensity);
	SunComponent->SetLightColor(LightProfile->SunColor);
	MoonComponent->SetIntensity(LightProfile->MoonIntensity);
	MoonComponent->SetLightColor(LightProfile->MoonColor);
}

