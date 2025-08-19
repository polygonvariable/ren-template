// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WeatherController.h"

// Engine Headers
#include "Materials/MaterialParameterCollectionInstance.h"

// Project Header
#include "RenCoreLibrary/Public/LogMacro.h"

#include "RenEnvironment/Public/Asset/EnvironmentProfileAsset.h"
#include "RenEnvironment/Public/Subsystem/EnvironmentSubsystem.h"

#include "RenWeather/Public/WeatherAsset.h"



void UWeatherController::SetMaterialCollection(UMaterialParameterCollection* MaterialCollection)
{
	if (!MaterialCollection)
	{
		LOG_ERROR(LogTemp, TEXT("MaterialCollection is invalid"));
		return;
	}

	MaterialCollectionInstance = GetWorld()->GetParameterCollectionInstance(MaterialCollection);
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

void UWeatherController::CleanUpItems()
{
	MaterialCollectionInstance = nullptr;
	CurrentWeatherAsset = nullptr;

	Super::CleanUpItems();
}

void UWeatherController::HandleItemChanged(UObject* Item)
{
	if (!IsValid(Item) || !MaterialCollectionInstance)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Item or MaterialCollection is invalid"));
		return;
	}

	UWeatherAsset* WeatherAsset = Cast<UWeatherAsset>(Item);
	if (!IsValid(WeatherAsset) || WeatherAsset == CurrentWeatherAsset)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("WeatherAsset is invalid or already active"));
		return;
	}

	UEnvironmentSubsystem* EnvironmentSubsystem = GetWorld()->GetSubsystem<UEnvironmentSubsystem>();
	if (IsValid(EnvironmentSubsystem))
	{
		for (auto& Kvp : WeatherAsset->EnvironmentProfiles)
		{
			if (IsValid(Kvp.Key))
			{
				EnvironmentSubsystem->AddStackedProfile(Kvp.Key, Kvp.Value);
			}
		}
	}

	HandleScalarTransition(TEXT("WeatherAlpha"), WeatherAsset->MaterialAlpha, 1.0f);
	HandleScalarTransition(TEXT("WeatherSpecular"), WeatherAsset->MaterialSpecular, 1.0f);
	HandleScalarTransition(TEXT("WeatherRoughness"), WeatherAsset->MaterialRoughness, 1.0f);
	HandleScalarTransition(TEXT("WeatherOpacity"), WeatherAsset->MaterialOpacity, 1.0f);
	HandleScalarTransition(TEXT("WeatherWind"), WeatherAsset->MaterialWind, 1.0f);
	HandleVectorTransition(TEXT("WeatherColor"), WeatherAsset->MaterialColor, 1.0f);

	CurrentWeatherAsset = WeatherAsset;

	OnWeatherChanged.Broadcast(WeatherAsset);

	PRINT_WARNING(LogTemp, 1.0f, TEXT("WeatherAsset changed"));
}


void UWeatherController::HandleItemRemoved(UObject* Item, bool bWasReplaced)
{
	UWeatherAsset* WeatherAsset = Cast<UWeatherAsset>(Item);
	if (!IsValid(WeatherAsset))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("WeatherAsset is invalid"));
		return;
	}

	UEnvironmentSubsystem* EnvironmentSubsystem = GetWorld()->GetSubsystem<UEnvironmentSubsystem>();
	if (IsValid(EnvironmentSubsystem))
	{
		for (const auto& Kvp : WeatherAsset->EnvironmentProfiles)
		{
			if (Kvp.Key)
			{
				EnvironmentSubsystem->RemoveStackedProfile(Kvp.Key, Kvp.Value);
			}
		}
	}

	OnWeatherRemoved.Broadcast(WeatherAsset);

	if (bWasReplaced)
	{
		PRINT_WARNING(LogTemp, 1.0f, TEXT("WeatherAsset was replaced"));
	}
	else
	{
		PRINT_WARNING(LogTemp, 1.0f, TEXT("WeatherAsset removed"));
	}
}


void UWeatherController::HandleNoItemsLeft()
{
	CurrentWeatherAsset = nullptr;

	PRINT_ERROR(LogTemp, 1.0f, TEXT("Weather controller has no items left, which was not supposed to happen"));
}

