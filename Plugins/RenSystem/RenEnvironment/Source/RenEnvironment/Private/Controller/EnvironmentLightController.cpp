// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Controller/EnvironmentLightController.h"

// Engine Headers
#include "Components/DirectionalLightComponent.h"
#include "EngineUtils.h"

// Project Headers
#include "RenCoreLibrary/Public/LogMacro.h"

#include "RenEnvironment/Public/Asset/EnvironmentProfileAsset.h"
#include "RenEnvironment/Public/Profile/EnvironmentProfileType.h"



UEnvironmentLightController::UEnvironmentLightController()
{
	EnvironmentProfileType = EEnvironmentProfileType::Light;
}

void UEnvironmentLightController::InitializeController()
{
	bool bSunFound = false;
	bool bMoonFound = false;

	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (IsValid(*ActorItr) && ActorItr->ActorHasTag(ActorTag))
		{
			TArray<UActorComponent*> SunComponents = ActorItr->GetComponentsByTag(UDirectionalLightComponent::StaticClass(), SunComponentTag);
			if (SunComponents.IsValidIndex(0) && IsValid(SunComponents[0]))
			{
				SunComponent = Cast<UDirectionalLightComponent>(SunComponents[0]);
				bSunFound = true;
			}

			TArray<UActorComponent*> MoonComponents = ActorItr->GetComponentsByTag(UDirectionalLightComponent::StaticClass(), MoonComponentTag);
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

void UEnvironmentLightController::CleanupController()
{
	SunComponent.Reset();
	MoonComponent.Reset();
}

void UEnvironmentLightController::HandleItemChanged(UObject* Item)
{
	UDirectionalLightComponent* SunComponentPtr = SunComponent.Get();
	UDirectionalLightComponent* MoonComponentPtr = MoonComponent.Get();

	if (IsValid(SunComponentPtr) && IsValid(MoonComponentPtr))
	{
		UEnvironmentLightProfileAsset* LightProfile = Cast<UEnvironmentLightProfileAsset>(Item);
		if (IsValid(LightProfile))
		{
			SunComponentPtr->SetIntensity(LightProfile->SunIntensity);
			SunComponentPtr->SetLightColor(LightProfile->SunColor);
			MoonComponentPtr->SetIntensity(LightProfile->MoonIntensity);
			MoonComponentPtr->SetLightColor(LightProfile->MoonColor);
		}
		else
		{
			PRINT_ERROR(LogTemp, 1.0f, TEXT("LightProfile asset is invalid"));
		}
	}
	else
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Sun or Moon not found"));
	}
}

