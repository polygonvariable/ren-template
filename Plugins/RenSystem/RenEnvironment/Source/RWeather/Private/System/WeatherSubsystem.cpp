// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/WeatherSubsystem.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "Actor/WeatherEffectManagerActor.h"
#include "Core/AssetManagerUtil.h"
#include "Core/EnvironmentSettings.h"
#include "Core/WeatherSettings.h"
#include "Data/WeatherAsset.h"
#include "Data/WeatherWorldConfig.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "System/WeatherController.h"
#include "WorldFragmentSettings.h"


bool UWeatherSubsystem::AddWeather(UWeatherAsset* WeatherAsset, int Priority)
{
	if (!IsValid(WeatherController) || !IsValid(WeatherAsset) || Priority <= 0)
	{
		LOG_ERROR(LogWeather, TEXT("WeatherController, WeatherAsset, Priority is invalid"));
		return false;
	}
	return WeatherController->AddWeather(WeatherAsset, Priority);
}

bool UWeatherSubsystem::RemoveWeather(int Priority)
{
	if (!IsValid(WeatherController) || Priority <= 0)
	{
		LOG_ERROR(LogWeather, TEXT("WeatherController, Priority is invalid"));
		return false;
	}
	return WeatherController->RemoveWeather(Priority);
}


UWeatherController* UWeatherSubsystem::GetWeatherController() const
{
	return WeatherController.Get();
}


bool UWeatherSubsystem::CreateWeatherTimer(float RefreshTime)
{
	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();

	TimerManager.SetTimer(WeatherTimer, this, &UWeatherSubsystem::HandleOnWeatherTimerTick, FMath::Max(5.0f, RefreshTime), true);
	return true;
}

void UWeatherSubsystem::RemoveWeatherTimer()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(WeatherTimer);
	WeatherTimer.Invalidate();
}


bool UWeatherSubsystem::CreateWeatherController(UClass* ControllerClass, UMaterialParameterCollection* MPC)
{
	if (IsValid(WeatherController) || !IsValid(ControllerClass))
	{
		LOG_ERROR(LogWeather, TEXT("WeatherController is already valid or WeatherController Class is invalid"));
		return false;
	}

	WeatherController = NewObject<UWeatherController>(this, ControllerClass);
	if (!IsValid(WeatherController))
	{
		LOG_ERROR(LogWeather, TEXT("Failed to create WeatherController"));
		return false;
	}

	WeatherController->Initialize(GetWorld()->GetParameterCollectionInstance(MPC));
	return true;
}

void UWeatherSubsystem::RemoveWeatherController()
{
	UWeatherController* Controller = WeatherController.Get();
	if (IsValid(Controller))
	{
		Controller->Deinitialize();
		Controller->MarkAsGarbage();
	}
	WeatherController = nullptr;
}


void UWeatherSubsystem::RegisterDefaultWeather(const FPrimaryAssetId& AssetId, int Priority)
{
	UWeatherAsset* WeatherAsset = AssetManager->GetPrimaryAssetObject<UWeatherAsset>(AssetId);
	if (!IsValid(WeatherAsset))
	{
		LOG_ERROR(LogWeather, TEXT("WeatherAsset is invalid"));
		return;
	}
	AddWeather(WeatherAsset, Priority);
}


void UWeatherSubsystem::HandleOnWeatherTimerTick()
{
	OnWeatherRefreshed.Broadcast();
}

void UWeatherSubsystem::HandleOnWeatherLoaded()
{
	FAssetManagerUtil::CancelHandle(WeatherHandle);
	
	const UWeatherWorldConfig* Config = UWeatherWorldConfig::Get(GetWorld());
	if (!IsValid(Config))
	{
		LOG_ERROR(LogWeather, TEXT("WeatherFragmentData is invalid"));
		return;
	}

	if (!CreateWeatherController(Config->WeatherController, Config->WeatherMPC))
	{
		LOG_ERROR(LogWeather, TEXT("Failed to create WeatherController"));
		return;
	}
	
	if (!CreateWeatherTimer(Config->RefreshDuration))
	{
		LOG_ERROR(LogWeather, TEXT("Failed to create WeatherTimer"));
		return;
	}

	if (IsValid(Config->EffectManager))
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		EffectManager = GetWorld()->SpawnActor<AWeatherEffectManagerActor>(Config->EffectManager, SpawnParameters);
	}

	RegisterDefaultWeather(Config->DefaultWeather, Config->DefaultPriority);
}


bool UWeatherSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

bool UWeatherSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Super::ShouldCreateSubsystem(Outer) || GetClass() != UWeatherSettings::Get()->SubsystemClass)
	{
		return false;
	}

	const UWeatherWorldConfig* Config = UWeatherWorldConfig::Get(Cast<UWorld>(Outer));
	if (!IsValid(Config))
	{
		return false;
	}

	return Config->bEnabled;
}

void UWeatherSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogWeather, TEXT("WeatherSubsystem Initialized"));

	AssetManager = UAssetManager::GetIfInitialized();
}

void UWeatherSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	Super::OnWorldComponentsUpdated(InWorld);
	LOG_WARNING(LogWeather, TEXT("WeatherSubsystem OnWorldComponentsUpdated"));

	const UWeatherWorldConfig* Config = UWeatherWorldConfig::Get(&InWorld);
	if (!IsValid(Config) || !Config->bEnabled)
	{
		LOG_ERROR(LogWeather, TEXT("WeatherFragmentData is invalid or disabled"));
		return;
	}

	FAssetManagerUtil::CancelHandle(WeatherHandle);

	const UEnvironmentSettings* Settings = UEnvironmentSettings::Get();
	const TArray<FName>& Bundles = Settings->EnvironmentBundles;
	const FPrimaryAssetId Weather = Config->DefaultWeather;

	WeatherHandle = AssetManager->LoadPrimaryAsset(Weather, Bundles, FStreamableDelegate::CreateUObject(this, &UWeatherSubsystem::HandleOnWeatherLoaded));
}

void UWeatherSubsystem::OnWorldEndPlay(UWorld& InWorld)
{
	RemoveWeatherTimer();
	RemoveWeatherController();

	if (IsValid(EffectManager) && !EffectManager->IsActorBeingDestroyed())
	{
		EffectManager->Destroy();
	}
	EffectManager = nullptr;

	Super::OnWorldEndPlay(InWorld);
}

void UWeatherSubsystem::Deinitialize()
{
	FAssetManagerUtil::CancelHandle(WeatherHandle);
	AssetManager = nullptr;

	LOG_WARNING(LogWeather, TEXT("WeatherSubsystem Deinitialized"));
	Super::Deinitialize();
}


UWeatherSubsystem* UWeatherSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return World->GetSubsystem<UWeatherSubsystem>();
}

