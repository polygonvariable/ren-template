// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "SeasonSubsystem.h"

// Engine Headers
#include "Engine/AssetManager.h"
#include "EngineUtils.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollectionInstance.h"

// Project Headers
#include "RCoreAssetManager/Public/RAssetManager.h"
#include "RCoreAssetManager/Private/RAssetManager.inl"
#include "RCoreMaterial/Public/MaterialSurfaceProperty.h"

#include "RCoreClock/Public/ClockManagerInterface.h"
#include "RCoreLibrary/Public/AssetManagerUtils.h"
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"
#include "RCoreLibrary/Public/SubsystemUtils.h"
#include "RCoreSettings/Public/WorldConfigSettings.h"

#include "RenEnvironment/Public/Asset/EnvironmentAsset.h"

#include "RenSeason/Public/SeasonAsset.h"



void USeasonSubsystem::HandleScalarTransition(FName ParameterName, float Target, float Alpha)
{
	if (!MPC)
	{
		return;
	}

	float Current = 0.0f;
	bool bFound = MPC->GetScalarParameterValue(ParameterName, Current);
	if (bFound)
	{
		MPC->SetScalarParameterValue(ParameterName, FMath::InterpEaseInOut(Current, Target, Alpha, 3.0f));
	}
}

void USeasonSubsystem::HandleVectorTransition(FName ParameterName, const FLinearColor& Target, float Alpha)
{
	if (!MPC)
	{
		return;
	}

	FLinearColor Current = FLinearColor::Transparent;
	bool bFound = MPC->GetVectorParameterValue(ParameterName, Current);
	if (bFound)
	{
		MPC->SetVectorParameterValue(ParameterName, FMath::InterpEaseInOut(Current, Target, Alpha, 3.0f));
	}
}

USeasonAsset* USeasonSubsystem::CalculateSeasonBlend(int CurrentDay, int TotalDays, float& OutAlpha, float& OutCurve) const
{
	int SeasonCount = LoadedSeasons.Num();
	int ClampedDay = FMath::Clamp(CurrentDay, 1, TotalDays);

	for (USeasonAsset* Season : LoadedSeasons)
	{
		int StartDay = Season->StartDay;
		int EndDay = Season->EndDay;

		if (ClampedDay >= StartDay && ClampedDay <= EndDay)
		{
			int DaySpan = EndDay - StartDay;
			if (DaySpan <= 0)
			{
				LOG_ERROR(LogSeason, TEXT("Invalid season range"));

				OutAlpha = 0.0f;
				OutCurve = 0.0f;
				return Season;
			}

			OutAlpha = float(ClampedDay - StartDay) / float(DaySpan);

			UCurveFloat* Curve = Season->SeasonProgressCurve;
			if (IsValid(Curve))
			{
				OutCurve = Curve->GetFloatValue(OutAlpha);
			}
			else
			{
				LOG_ERROR(LogSeason, TEXT("Curve Asset is invalid, incorrect season transition is expected"));
				OutCurve = 0.0f;
			}

			return Season;
		}
	}

	OutAlpha = 0.0f;
	OutCurve = 0.0f;

	return nullptr;
}

void USeasonSubsystem::LoadDefaultSeasons(const TArray<FPrimaryAssetId>& AssetIds)
{
	if (!IsValid(AssetManager) || AssetIds.IsEmpty())
	{
		LOG_ERROR(LogWeather, TEXT("AssetManager is invalid or AssetIds is empty"));
		return;
	}

	TFuture<FLatentResultAssets<USeasonAsset>> Future = AssetManager->FetchPrimaryAssets<USeasonAsset>(AssetIds);
	if (Future.IsValid())
	{
		LOG_ERROR(LogWeather, TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<USeasonSubsystem> WeakThis(this);
	Future.Next([WeakThis, AssetIds](const FLatentResultAssets<USeasonAsset>& Result)
		{
			USeasonSubsystem* This = WeakThis.Get();
			if (IsValid(This) && Result.IsValid())
			{
				This->HandleSeasonsLoaded(Result.Assets);
			}
		}
	);
}


void USeasonSubsystem::HandleDayChanged(int CurrentDay)
{
	IClockManagerInterface* ClockManager = ClockManagerInterface.Get();
	if (!MPC || !ClockManager)
	{
		LOG_ERROR(LogSeason, TEXT("MPC, ClockManager is invalid"));
		return;
	}

	float Alpha = 0.0f, Curve = 0.0f;
	int TotalDays = ClockManager->GetTotalDaysInAYear();

	USeasonAsset* SeasonAsset = CalculateSeasonBlend(CurrentDay, TotalDays, Alpha, Curve);
	if (!IsValid(SeasonAsset))
	{
		LOG_ERROR(LogSeason, TEXT("SeasonAsset is invalid"));
		return;
	}

	if (CurrentSeason != SeasonAsset)
	{
		if (IsValid(CurrentSeason))
		{
			Delegates.OnEnded.Broadcast(CurrentSeason);
		}

		Delegates.OnStarted.Broadcast(SeasonAsset);

		CurrentSeason = SeasonAsset;
	}

	FMaterialSurfaceProperty& SurfaceProperty = SeasonAsset->SurfaceProperty;

	HandleScalarTransition(TEXT("SeasonSpecular"), SurfaceProperty.Specular, 1.0f);
	HandleScalarTransition(TEXT("SeasonRoughness"), SurfaceProperty.Roughness, 1.0f);
	HandleScalarTransition(TEXT("SeasonOpacity"), SurfaceProperty.Opacity, 1.0f);
	HandleVectorTransition(TEXT("SeasonTint"), SurfaceProperty.Tint, 1.0f);

	LOG_INFO(LogSeason, TEXT("Season transition"));
}

void USeasonSubsystem::HandleSeasonsLoaded(const TArray<USeasonAsset*>& Assets)
{
	LoadedSeasons.Append(Assets);

	IClockManagerInterface* ClockManager = SubsystemUtils::GetSubsystemInterface<UWorld, UWorldSubsystem, IClockManagerInterface>(GetWorld());
	if (!ClockManager)
	{
		LOG_ERROR(LogSeason, TEXT("ClockManager is invalid"));
		return;
	}

	ClockManager->GetClockDelegates().OnDayChanged.AddUObject(this, &USeasonSubsystem::HandleDayChanged);

	ClockManagerInterface = TWeakInterfacePtr<IClockManagerInterface>(ClockManager);
}


UPrimaryDataAsset* USeasonSubsystem::GetCurrentSeason() const
{
	return CurrentSeason;
}

FSeasonDelegates& USeasonSubsystem::GetSeasonDelegates()
{
	return Delegates;
}


bool USeasonSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return
		WorldType == EWorldType::Game ||
		WorldType == EWorldType::PIE;
}

void USeasonSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogEnvironment, TEXT("Initialized"));

	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());
}

void USeasonSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	Super::OnWorldComponentsUpdated(InWorld);
	LOG_WARNING(LogSeason, TEXT("OnWorldComponentsUpdated"));

	AWorldConfigSettings* WorldSettings = Cast<AWorldConfigSettings>(InWorld.GetWorldSettings());
	if (!IsValid(WorldSettings))
	{
		LOG_ERROR(LogSeason, TEXT("WorldConfigSettings is invalid"));
		return;
	}

	UEnvironmentAsset* EnvironmentAsset = Cast<UEnvironmentAsset>(WorldSettings->EnvironmentAsset);
	if (!IsValid(EnvironmentAsset))
	{
		LOG_ERROR(LogSeason, TEXT("EnvironmentAsset is invalid"));
		return;
	}

	bool bSeasonEnabled = EnvironmentAsset->bSeasonEnabled;
	UMaterialParameterCollection* ParameterCollection = EnvironmentAsset->SeasonParameterCollection.Get();

	if (!bSeasonEnabled || !ParameterCollection)
	{
		LOG_ERROR(LogSeason, TEXT("Season is disabled, ParameterCollection is invalid"));
		return;
	}
	
	MPC = InWorld.GetParameterCollectionInstance(ParameterCollection);
	if (!IsValid(MPC))
	{
		LOG_ERROR(LogSeason, TEXT("Season MPC is invalid"));
		return;
	}

	LoadDefaultSeasons(EnvironmentAsset->DefaultSeasons);
}

void USeasonSubsystem::Deinitialize()
{
	IClockManagerInterface* ClockManager = ClockManagerInterface.Get();
	if (ClockManager)
	{
		ClockManager->GetClockDelegates().OnDayChanged.RemoveAll(this);
	}
	ClockManagerInterface.Reset();

	LoadedSeasons.Empty();

	AssetManager = nullptr;
	CurrentSeason = nullptr;
	MPC = nullptr;

	LOG_WARNING(LogEnvironment, TEXT("Deinitialized"));
	Super::Deinitialize();
}

