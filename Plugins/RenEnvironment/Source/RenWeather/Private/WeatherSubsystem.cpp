// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WeatherSubsystem.h"

// Engine Header

// Project Header
#include "RenCore/Public/Library/MiscLibrary.h"
#include "RenCore/Public/Macro/LogMacro.h"

#include "RenEnvironment/Public/Asset/EnvironmentAsset.h"
#include "RenEnvironment/Public/EnvironmentWorldSettings.h"

#include "RenWeather/Public/WeatherAsset.h"
#include "RenWeather/Public/WeatherController.h"



bool UWeatherSubsystem::AddWeather(UWeatherAsset* WeatherAsset, int Priority)
{
	if (!IsValid(WeatherController) || !IsValid(WeatherAsset))
	{
		LOG_ERROR(LogTemp, "WeatherController or WeatherAsset is not valid");
		return false;
	}
	return WeatherController->AddItem(WeatherAsset, Priority);
}

bool UWeatherSubsystem::RemoveWeather(int Priority)
{
	if (!IsValid(WeatherController))
	{
		LOG_ERROR(LogTemp, "WeatherController is not valid");
		return false;
	}
	return WeatherController->RemoveItem(Priority);
}

UWeatherController* UWeatherSubsystem::GetWeatherController() const
{
	return WeatherController;
}



void UWeatherSubsystem::CreateWeatherTimer()
{
	TimerUtils::StartTimer(WeatherTimerHandle, this, &UWeatherSubsystem::HandleWeatherTimer, WeatherChangeTime);
}


bool UWeatherSubsystem::CreateWeatherController()
{
	if (IsValid(WeatherController))
	{
		LOG_WARNING(LogTemp, "Weather Controller is already valid");
		return false;
	}

	WeatherController = NewObject<UWeatherController>(this);
	if (!IsValid(WeatherController))
	{
		LOG_ERROR(LogTemp, "Failed to create Weather Controller");
		return false;
	}

	WeatherController->OnWeatherChanged.AddUObject(this, &UWeatherSubsystem::HandleItemChanged);
	WeatherController->OnWeatherRemoved.AddUObject(this, &UWeatherSubsystem::HandleItemRemoved);

	return true;
}



void UWeatherSubsystem::CreateWeatherMaterialCollection()
{
	if (!IsValid(EnvironmentAsset) || !IsValid(WeatherController))
	{
		LOG_ERROR(LogTemp, TEXT("EnvironmentAsset or WeatherController is invalid"));
		return;
	}
	WeatherController->SetMaterialCollection(EnvironmentAsset->WeatherMaterialParameter);
}



void UWeatherSubsystem::HandleWeatherTimer()
{
	PRINT_WARNING(LogTemp, 1.0f, TEXT("Weather changed"));
	OnWeatherCanChange.Broadcast();
}

void UWeatherSubsystem::HandleItemChanged(UWeatherAsset* WeatherAsset)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TSet<TSubclassOf<AWeatherEffectActor>> WeatherEffects = WeatherAsset->WeatherEffects;
	for(auto& WeatherEffect : WeatherEffects)
	{
		AWeatherEffectActor* EffectActor = EffectActors.FindRef(WeatherEffect);
		if (IsValid(EffectActor))
		{
			EffectActor->ActivateEffect();
		}
		else
		{
			AWeatherEffectActor* NewEffectActor = GetWorld()->SpawnActor<AWeatherEffectActor>(WeatherEffect, SpawnParameters);
			if (IsValid(NewEffectActor))
			{
				NewEffectActor->ActivateEffect();
				EffectActors.Add(WeatherEffect, NewEffectActor);
			}
		}
	}
}

void UWeatherSubsystem::HandleItemRemoved(UWeatherAsset* WeatherAsset)
{
	TSet<TSubclassOf<AWeatherEffectActor>> WeatherEffects = WeatherAsset->WeatherEffects;
	for (auto& WeatherEffect : WeatherEffects)
	{
		AWeatherEffectActor* EffectActor = EffectActors.FindRef(WeatherEffect);
		if (IsValid(EffectActor))
		{
			EffectActor->DeactivateEffect();
		}
	}
}

bool UWeatherSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UWeatherSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("WeatherSubsystem initialized"));
}

void UWeatherSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	Super::OnWorldComponentsUpdated(InWorld);

	AEnvironmentWorldSettings* WorldSettings = Cast<AEnvironmentWorldSettings>(InWorld.GetWorldSettings());
	if (!IsValid(WorldSettings))
	{
		LOG_ERROR(LogTemp, TEXT("EnvironmentWorldSettings is not valid"));
		return;
	}

	EnvironmentAsset = WorldSettings->EnvironmentAsset;
	if (!IsValid(EnvironmentAsset))
	{
		LOG_ERROR(LogTemp, TEXT("EnvironmentAsset is not valid"));
		return;
	}

	if (CreateWeatherController())
	{
		CreateWeatherTimer();
		CreateWeatherMaterialCollection();

		UPrimaryDataAsset* PrimaryWeatherAsset = EnvironmentAsset->DefaultWeather;
		if (IsValid(PrimaryWeatherAsset))
		{
			UWeatherAsset* DefaultWeatherAsset = Cast<UWeatherAsset>(PrimaryWeatherAsset);
			if (IsValid(DefaultWeatherAsset))
			{
				AddWeather(DefaultWeatherAsset, 0);
			}
		}
	}
}

void UWeatherSubsystem::Deinitialize()
{
	if (IsValid(WeatherController))
	{
		WeatherController->OnWeatherChanged.RemoveAll(this);
		WeatherController->OnWeatherRemoved.RemoveAll(this);
		WeatherController->CleanUpItems();
		WeatherController->MarkAsGarbage();
	}
	WeatherController = nullptr;

	for(auto& EffectActor : EffectActors)
	{
		if (IsValid(EffectActor.Value))
		{
			EffectActor.Value->Destroy();
		}
	}
	EffectActors.Empty();

	TimerUtils::ClearTimer(WeatherTimerHandle, this);

	LOG_WARNING(LogTemp, TEXT("WeatherSubsystem deinitialized"));
	Super::Deinitialize();
}

