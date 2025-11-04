// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WeatherSubsystem.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "RCoreAssetManager/Public/RAssetManager.h"
#include "RCoreAssetManager/Private/RAssetManager.inl"

#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"
#include "RCoreLibrary/Public/TimerUtils.h"
#include "RCoreSettings/Public/WorldConfigSettings.h"

#include "RenEnvironment/Public/Asset/EnvironmentAsset.h"

#include "RenWeather/Public/WeatherAsset.h"
#include "RenWeather/Public/WeatherController.h"



bool UWeatherSubsystem::AddWeather(UWeatherAsset* WeatherAsset, int Priority)
{
	if (!IsValid(WeatherController) || !IsValid(WeatherAsset))
	{
		LOG_ERROR(LogWeather, TEXT("WeatherController, WeatherAsset is invalid"));
		return false;
	}

	return WeatherController->AddItem(WeatherAsset, Priority);
}

bool UWeatherSubsystem::RemoveWeather(int Priority)
{
	if (!IsValid(WeatherController))
	{
		LOG_ERROR(LogWeather, TEXT("WeatherController is invalid"));
		return false;
	}

	return WeatherController->RemoveItem(Priority);
}

void UWeatherSubsystem::AddWeather(const FGuid& LatentId, const FPrimaryAssetId& AssetId, int Priority)
{
	if (!IsValid(AssetManager) || !UWeatherAsset::IsValid(AssetId))
	{
		LOG_ERROR(LogWeather, TEXT("AssetManager, AssetId is invalid"));
		return;
	}

	TFuture<FLatentResultAsset<UWeatherAsset>> Future = AssetManager->FetchPrimaryAsset<UWeatherAsset>(LatentId, AssetId);
	if (!Future.IsValid())
	{
		LOG_ERROR(LogWeather, TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UWeatherSubsystem> WeakThis(this);
	TFunction<void(const FLatentResultAsset<UWeatherAsset>&)> Callback = [WeakThis, Priority](const FLatentResultAsset<UWeatherAsset>& Result)
		{
			UWeatherSubsystem* This = WeakThis.Get();
			if (IsValid(This) && Result.IsValid())
			{
				This->AddWeather(Result.Asset, Priority);
			}
		};

	Future.Next(MoveTemp(Callback));
}

bool UWeatherSubsystem::RemoveWeather(const FGuid& LatentId, int Priority)
{
	if (IsValid(AssetManager))
	{
		AssetManager->CancelFetch(LatentId);
	}
	return RemoveWeather(Priority);
}


void UWeatherSubsystem::LoadWeatherManager(const FSoftClassPath& ClassPath)
{
	if (!IsValid(AssetManager))
	{
		return;
	}
	// Load & Spawn WeatherManager
}

void UWeatherSubsystem::LoadDefaultWeather(const FPrimaryAssetId& AssetId)
{
	if (!UWeatherAsset::IsValid(AssetId))
	{
		LOG_ERROR(LogWeather, TEXT("AssetId is invalid"));
		return;
	}
	
	TFuture<FLatentResultAsset<UWeatherAsset>> Future = AssetManager->FetchPrimaryAsset<UWeatherAsset>(AssetId);
	if (!Future.IsValid())
	{
		LOG_ERROR(LogWeather, TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UWeatherSubsystem> WeakThis(this);
	TFunction<void(const FLatentResultAsset<UWeatherAsset>&)> Callback = [WeakThis](const FLatentResultAsset<UWeatherAsset>& Result)
		{
			UWeatherSubsystem* This = WeakThis.Get();
			if (IsValid(This) && Result.IsValid())
			{
				This->AddWeather(Result.Asset, 0);
			}
		};

	Future.Next(MoveTemp(Callback));
}


bool UWeatherSubsystem::CreateWeatherTimer(float RefreshTime)
{
	return TimerUtils::StartTimer(WeatherTimer, this, &UWeatherSubsystem::HandleWeatherTimer, FMath::Max(5.0f, RefreshTime));
}

bool UWeatherSubsystem::CreateWeatherController(TSubclassOf<UObjectPrioritySystem> ControllerClass)
{
	if (IsValid(WeatherController) || !IsValid(ControllerClass))
	{
		LOG_ERROR(LogWeather, TEXT("WeatherController is already valid or WeatherController Class is invalid"));
		return false;
	}

	UWeatherController* Controller = NewObject<UWeatherController>(this, ControllerClass);
	if (!IsValid(Controller))
	{
		LOG_ERROR(LogWeather, TEXT("Failed to create WeatherController"));
		return false;
	}

	Controller->Initialize();

	FWeatherDelegates& WeatherDelegates = Controller->Delegates;
	WeatherDelegates.OnChanged.AddWeakLambda(this, [this](UPrimaryDataAsset* WeatherAsset) { Delegates.OnChanged.Broadcast(WeatherAsset); });
	WeatherDelegates.OnRemoved.AddWeakLambda(this, [this](UPrimaryDataAsset* WeatherAsset) { Delegates.OnRemoved.Broadcast(WeatherAsset); });

	WeatherController = Controller;

	return true;
}

bool UWeatherSubsystem::CreateWeatherMPC(UMaterialParameterCollection* Collection)
{
	if (!IsValid(WeatherController) || !Collection)
	{
		LOG_ERROR(LogWeather, TEXT("WeatherController, MPC is invalid"));
		return false;
	}

	UWorld* World = GetWorld();
	UMaterialParameterCollectionInstance* MPC = World->GetParameterCollectionInstance(Collection);
	if (!MPC)
	{
		LOG_ERROR(LogWeather, TEXT("Failed to create MPC"));
		return false;
	}

	WeatherController->SetMaterialCollection(MPC);
	return true;
}


void UWeatherSubsystem::HandleWeatherTimer()
{
	PRINT_INFO(LogWeather, 1.0f, TEXT("Weather refreshed"));
	Delegates.OnRefreshed.Broadcast();
}


bool UWeatherSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return
		WorldType == EWorldType::Game ||
		WorldType == EWorldType::PIE;
}

void UWeatherSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_INFO(LogWeather, TEXT("Initialized"));

	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());
}

void UWeatherSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	Super::OnWorldComponentsUpdated(InWorld);
	LOG_INFO(LogWeather, TEXT("OnWorldComponentsUpdated"));

	AWorldConfigSettings* WorldSettings = Cast<AWorldConfigSettings>(InWorld.GetWorldSettings());
	if (!IsValid(WorldSettings))
	{
		LOG_ERROR(LogWeather, TEXT("WorldConfigSettings is invalid"));
		return;
	}

	UEnvironmentAsset* EnvironmentAsset = Cast<UEnvironmentAsset>(WorldSettings->EnvironmentAsset);
	if (!IsValid(EnvironmentAsset))
	{
		LOG_ERROR(LogWeather, TEXT("EnvironmentAsset is invalid"));
		return;
	}

	if (!EnvironmentAsset->bWeatherEnabled)
	{
		LOG_ERROR(LogWeather, TEXT("Weather is disabled"));
		return;
	}
	
	if (!CreateWeatherController(EnvironmentAsset->WeatherController))
	{
		LOG_ERROR(LogWeather, TEXT("Failed to create WeatherController"));
		return;
	}

	if (!CreateWeatherMPC(EnvironmentAsset->WeatherParameterCollection))
	{
		LOG_ERROR(LogWeather, TEXT("Failed to create WeatherMaterialCollection"));
		return;
	}

	if (!CreateWeatherTimer(EnvironmentAsset->WeatherRefreshDuration))
	{
		LOG_ERROR(LogWeather, TEXT("Failed to create WeatherTimer"));
		return;
	}

	LoadDefaultWeather(EnvironmentAsset->DefaultWeather);
}

void UWeatherSubsystem::Deinitialize()
{
	TimerUtils::ClearTimer(WeatherTimer, this);

	UWeatherController* Controller = WeatherController.Get();
	if (IsValid(Controller))
	{
		Controller->Delegates.Clear();
		Controller->CleanUpItems();
		Controller->MarkAsGarbage();
	}
	WeatherController = nullptr;

	LOG_INFO(LogWeather, TEXT("Deinitialized"));
	Super::Deinitialize();
}


