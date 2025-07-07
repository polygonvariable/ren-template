// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WeatherController.h"

// Engine Headers
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollectionInstance.h"

// Project Header
#include "RenCore/Public/Macro/LogMacro.h"
#include "RenCore/Public/Timer/Timer.h"

#include "RenEnvironment/Public/Asset/EnvironmentProfileAsset.h"
#include "RenEnvironment/Public/Asset/WeatherAsset.h"
#include "RenEnvironment/Public/Subsystem/EnvironmentSubsystem.h"

#include "RenWeather/Public/WeatherProfile.h"



void UWeatherController::SetMaterialCollection(UMaterialParameterCollection* MaterialCollection)
{
	if (!MaterialCollection)
	{
		LOG_ERROR(LogTemp, TEXT("MaterialCollection is invalid"));
		return;
	}

	MaterialCollectionInstance = GetWorld()->GetParameterCollectionInstance(MaterialCollection);
}

void UWeatherController::CleanUpItems()
{
	MaterialCollectionInstance = nullptr;
	CurrentWeatherAsset = nullptr;
	CurrentWeatherName = NAME_None;

	Super::CleanUpItems();
}



void UWeatherController::HandleScalarTransition(FName ParameterName, float Target, float Alpha)
{
	if (!MaterialCollectionInstance) return;

	float Current = 0.0f;
	if (MaterialCollectionInstance->GetScalarParameterValue(ParameterName, Current))
	{
		MaterialCollectionInstance->SetScalarParameterValue(ParameterName, FMath::Lerp(Current, Target, Alpha));
	}
}

void UWeatherController::HandleVectorTransition(FName ParameterName, const FLinearColor& Target, float Alpha)
{
	if (!MaterialCollectionInstance) return;

	FLinearColor Current = FLinearColor::Transparent;
	if (MaterialCollectionInstance->GetVectorParameterValue(ParameterName, Current))
	{
		MaterialCollectionInstance->SetVectorParameterValue(ParameterName, FMath::CInterpTo(Current, Target, Alpha, 1.0f));
	}
}

void UWeatherController::HandleItemChanged(UObject* Item)
{
	if (!IsValid(Item) || !MaterialCollectionInstance)
	{
		LOG_ERROR(LogTemp, TEXT("Item or MaterialCollection is invalid"));
		return;
	}

	if (UWeatherAsset* WeatherAsset = Cast<UWeatherAsset>(Item))
	{
		if (WeatherAsset->WeatherName == CurrentWeatherName) return;

		if (UEnvironmentSubsystem* EnvironmentSubsystem = GetWorld()->GetSubsystem<UEnvironmentSubsystem>())
		{
			for (auto& Kvp : WeatherAsset->EnvironmentProfiles)
			{
				if (IsValid(Kvp.Key))
				{
					EnvironmentSubsystem->AddStackedProfile(Kvp.Key, Kvp.Value);
				}
			}
		}

		CurrentWeatherName = WeatherAsset->WeatherName;
		CurrentWeatherAsset = WeatherAsset;

		HandleScalarTransition(TEXT("WeatherAlpha"), WeatherAsset->MaterialAlpha, 1.0f);
		HandleScalarTransition(TEXT("WeatherSpecular"), WeatherAsset->MaterialSpecular, 1.0f);
		HandleScalarTransition(TEXT("WeatherRoughness"), WeatherAsset->MaterialRoughness, 1.0f);
		HandleScalarTransition(TEXT("WeatherOpacity"), WeatherAsset->MaterialOpacity, 1.0f);
		HandleVectorTransition(TEXT("WeatherColor"), WeatherAsset->MaterialColor, 1.0f);
	}
}


//void UWeatherController::HandleItemRemoved(UObject* Item)
//{
//	if (UWeatherAsset* WeatherAsset = Cast<UWeatherAsset>(Item))
//	{
//		if (UEnvironmentSubsystem* EnvironmentSubsystem = GetWorld()->GetSubsystem<UEnvironmentSubsystem>())
//		{
//			for (TPair<TObjectPtr<UEnvironmentProfileAsset>, int>& Kvp : WeatherAsset->EnvironmentProfiles)
//			{
//				if (Kvp.Key == nullptr) continue;
//				EnvironmentSubsystem->RemoveStackedProfile(Kvp.Key->ProfileType, Kvp.Value);
//			}
//		}
//	}
//}


void UWeatherController::HandleNoItemsLeft()
{
	CurrentWeatherName = NAME_None;
	CurrentWeatherAsset = nullptr;

	LOG_ERROR(LogTemp, TEXT("Weather controller has no items left, which was not supposed to happen"));
}
