// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/SeasonSubsystem.h"

// Engine Headers
#include "Engine/AssetManager.h"
#include "EngineUtils.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

// Project Headers
#include "Core/AssetManagerUtil.h"
#include "Core/EnvironmentSettings.h"
#include "Core/SeasonSettings.h"
#include "Core/SeasonSettings.h"
#include "Data/EnvironmentAsset.h"
#include "Data/SeasonAsset.h"
#include "Data/SeasonCollectionAsset.h"
#include "Data/SeasonWorldConfig.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Log/LogMacro.h"
#include "Manager/RAssetManager.inl"
#include "RCoreClock/Public/ClockManagerInterface.h"
#include "RCoreMaterial/Public/MaterialSurfaceProperty.h"
#include "RCoreSettings/Public/WorldFragmentSettings.h"
#include "System/SeasonController.h"
#include "Util/SubsystemUtil.h"


USeasonController* USeasonSubsystem::GetSeasonController() const
{
	return SeasonController;
}

bool USeasonSubsystem::CreateSeasonController(UClass* ControllerClass, UMaterialParameterCollection* MPC)
{
	if (IsValid(SeasonController) || !IsValid(ControllerClass) || !IsValid(MPC))
	{
		LOG_ERROR(LogSeason, TEXT("SeasonController is already valid or SeasonController Class, MPC is invalid"));
		return false;
	}

	SeasonController = NewObject<USeasonController>(this, ControllerClass);
	if (!IsValid(SeasonController))
	{
		LOG_ERROR(LogSeason, TEXT("Failed to create SeasonController"));
		return false;
	}

	SeasonController->Initialize(GetWorld()->GetParameterCollectionInstance(MPC));
	return true;
}

void USeasonSubsystem::RemoveSeasonController()
{
	if (IsValid(SeasonController))
	{
		SeasonController->Deinitialize();
		SeasonController->MarkAsGarbage();
	}
	SeasonController = nullptr;
}

void USeasonSubsystem::HandleOnSeasonLoaded()
{
	FAssetManagerUtil::CancelHandle(AssetHandle);

	const USeasonWorldConfig* Config = USeasonWorldConfig::Get(GetWorld());
	if (!IsValid(Config) || !Config->bEnabled)
	{
		LOG_ERROR(LogSeason, TEXT("Season world config is invalid or disabled"));
		return;
	}

	if (!CreateSeasonController(Config->SeasonController, Config->SeasonMPC))
	{
		LOG_ERROR(LogSeason, TEXT("Failed to create SeasonController"));
		return;
	}

	USeasonCollectionAsset* CollectionAsset = AssetManager->GetPrimaryAssetObject<USeasonCollectionAsset>(Config->DefaultSeason);
	if (IsValid(CollectionAsset))
	{
		SeasonController->AddSeason(CollectionAsset, Config->DefaultPriority);
	}
}

bool USeasonSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

bool USeasonSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Super::ShouldCreateSubsystem(Outer) || GetClass() != USeasonSettings::Get()->SubsystemClass)
	{
		return false;
	}

	const USeasonWorldConfig* Config = USeasonWorldConfig::Get(Cast<UWorld>(Outer));
	if (!IsValid(Config))
	{
		return false;
	}

	return Config->bEnabled;
}

void USeasonSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogSeason, TEXT("SeasonSubsystem Initialized"));

	AssetManager = UAssetManager::GetIfInitialized();
}

void USeasonSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	LOG_WARNING(LogSeason, TEXT("SeasonSubsystem OnWorldComponentsUpdated"));

	const USeasonWorldConfig* Config = USeasonWorldConfig::Get(&InWorld);
	if (!IsValid(Config) || !Config->bEnabled)
	{
		LOG_ERROR(LogWeather, TEXT("WeatherFragmentData is invalid or disabled"));
		return;
	}

	FAssetManagerUtil::CancelHandle(AssetHandle);

	const UEnvironmentSettings* Settings = UEnvironmentSettings::Get();
	const TArray<FName>& Bundles = Settings->EnvironmentBundles;
	const FPrimaryAssetId Weather = Config->DefaultSeason;

	AssetHandle = AssetManager->LoadPrimaryAsset(Weather, Bundles, FStreamableDelegate::CreateUObject(this, &USeasonSubsystem::HandleOnSeasonLoaded));
}

void USeasonSubsystem::Deinitialize()
{
	FAssetManagerUtil::CancelHandle(AssetHandle);
	AssetManager = nullptr;

	LOG_WARNING(LogSeason, TEXT("SeasonSubsystem Deinitialized"));
	Super::Deinitialize();
}

USeasonSubsystem* USeasonSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return World->GetSubsystem<USeasonSubsystem>();
}

