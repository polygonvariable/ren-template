// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/SeasonSubsystem.h"

// Engine Headers
#include "Engine/AssetManager.h"
#include "Materials/MaterialParameterCollection.h"

// Project Headers
#include "Core/AssetManagerUtil.h"
#include "Core/EnvironmentSettings.h"
#include "Core/SeasonSettings.h"
#include "Data/SeasonCollectionAsset.h"
#include "Data/SeasonWorldConfig.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "RCoreSettings/Public/WorldFragmentSettings.h"
#include "System/SeasonController.h"


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

	if (!IsValid(SeasonConfig))
	{
		LOG_ERROR(LogSeason, TEXT("Season world config is invalid or disabled"));
		return;
	}

	if (!CreateSeasonController(SeasonConfig->SeasonController, SeasonConfig->SeasonMPC))
	{
		LOG_ERROR(LogSeason, TEXT("Failed to create SeasonController"));
		return;
	}

	USeasonCollectionAsset* CollectionAsset = AssetManager->GetPrimaryAssetObject<USeasonCollectionAsset>(SeasonConfig->DefaultSeason);
	if (IsValid(CollectionAsset))
	{
		SeasonController->AddSeasonCollection(CollectionAsset, SeasonConfig->DefaultPriority);
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
	const USeasonWorldConfig* Config = AWorldFragmentSettings::GetConfigByClass<USeasonWorldConfig>(Cast<UWorld>(Outer));
	return IsValid(Config) && Config->bEnabled;
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

	SeasonConfig = AWorldFragmentSettings::GetConfigByClass<USeasonWorldConfig>(&InWorld);
	if (!IsValid(SeasonConfig) || !SeasonConfig->bEnabled)
	{
		LOG_ERROR(LogWeather, TEXT("WeatherFragmentData is invalid or disabled"));
		return;
	}

	FAssetManagerUtil::CancelHandle(AssetHandle);

	const UEnvironmentSettings* Settings = UEnvironmentSettings::Get();
	const TArray<FName>& Bundles = Settings->EnvironmentBundles;
	const FPrimaryAssetId Season = SeasonConfig->DefaultSeason;

	AssetHandle = AssetManager->LoadPrimaryAsset(Season, Bundles, FStreamableDelegate::CreateUObject(this, &USeasonSubsystem::HandleOnSeasonLoaded));
}

void USeasonSubsystem::OnWorldEndPlay(UWorld& InWorld)
{
	RemoveSeasonController();

	FAssetManagerUtil::CancelHandle(AssetHandle);
	AssetManager = nullptr;
	SeasonConfig = nullptr;

	LOG_WARNING(LogSeason, TEXT("SeasonSubsystem OnWorldEndPlay"));
}

USeasonSubsystem* USeasonSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return World->GetSubsystem<USeasonSubsystem>();
}

