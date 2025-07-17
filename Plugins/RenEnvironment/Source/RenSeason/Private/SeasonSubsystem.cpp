// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "SeasonSubsystem.h"

// Engine Header
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollectionInstance.h"

// Project Header
#include "RenCore/Public/Interface/ClockProviderInterface.h"
#include "RenAsset/Public/Game/ClockAsset.h"
#include "RenCore/Public/Developer/GameMetadataSettings.h"
#include "RenCore/Public/Macro/LogMacro.h"
#include "RenCore/Public/Library/MiscLibrary.h"
#include "RenCore/Public/WorldConfigSettings.h"

#include "RenEnvironment/Public/Asset/EnvironmentAsset.h"

#include "RenSeason/Public/SeasonAsset.h"



const FName USeasonSubsystem::GetSeasonName() const
{
	return (IsValid(CurrentSeason)) ? CurrentSeason->SeasonName : NAME_None;
}

UPrimaryDataAsset* USeasonSubsystem::GetSeasonAsset()
{
	return CurrentSeason;
}

FOnSeasonChanged& USeasonSubsystem::GetOnSeasonChanged()
{
	return OnSeasonChanged;
}


USeasonAsset* USeasonSubsystem::GetSeasonAlpha(int CurrentDay, float& OutAlpha, float& OutCurveAlpha) const
{
	int SeasonCount = SeasonAssets.Num();
	int ClampedDay = FMath::Clamp(CurrentDay, 1, TotalDaysInAYear);

	for (int i = 0; i < SeasonCount; i++)
	{
		USeasonAsset* Season = SeasonAssets[i];

		if (ClampedDay >= Season->SeasonStartDay && ClampedDay <= Season->SeasonEndDay)
		{
			int DaySpan = Season->SeasonEndDay - Season->SeasonStartDay;
			if (DaySpan <= 0)
			{
				PRINT_ERROR(LogTemp, 2.0f, TEXT("Invalid season range"));

				OutAlpha = 0.0f;
				OutCurveAlpha = 0.0f;
				return Season;
			}

			OutAlpha = float(ClampedDay - Season->SeasonStartDay) / float(DaySpan);
			if (UCurveFloat* Curve = Season->SeasonCurve)
			{
				OutCurveAlpha = Curve->GetFloatValue(OutAlpha);
			}
			else
			{
				PRINT_ERROR(LogTemp, 2.0f, TEXT("Curve Asset is invalid, incorrect season transition is expected"));
				OutCurveAlpha = 0.0f;
			}

			return Season;
		}
	}

	OutAlpha = 0.0f;
	OutCurveAlpha = 0.0f;

	return nullptr;
}



bool USeasonSubsystem::IsSeasonsValid() const
{
	/*if (!IsValid(EnvironmentAsset) || !IsValid(ClockAsset))
	{
		LOG_ERROR(LogTemp, TEXT("EnvironmentAsset or ClockAsset is invalid"));
		return false;
	}


	TArray<USeasonAsset*>& SeasonAssets = EnvironmentAsset->DefaultSeasons;

	int SeasonCount = SeasonAssets.Num();
	if (SeasonCount == 0)
	{
		LOG_ERROR(LogTemp, TEXT("Seasons are empty"));
		return false;
	}

	for (int i = 0; i < SeasonCount; i++)
	{
		USeasonAsset* Current = SeasonAssets[i];
		if (!IsValid(Current))
		{
			LOG_ERROR(LogTemp, TEXT("Season asset is not valid"));
			return false;
		}

		if (Current->SeasonStartDay > Current->SeasonEndDay)
		{
			LOG_ERROR(LogTemp, TEXT("Season's start day is greater than end day"));
			return false;
		}

		if (i > 0) {
			USeasonAsset* Previous = SeasonAssets[i - 1];
			if (Current->SeasonStartDay != Previous->SeasonEndDay + 1)
			{
				LOG_ERROR(LogTemp, TEXT("Invalid gap between seasons, %s"), *Previous->SeasonName.ToString());
				return false;
			}
		}
	}

	USeasonAsset* FirstSeason = SeasonAssets[0];
	USeasonAsset* LastSeason = SeasonAssets[SeasonCount - 1];
	if (FirstSeason->SeasonStartDay != 1 || LastSeason->SeasonEndDay != ClockAsset->TotalDaysInAYear) {
		LOG_ERROR(LogTemp, TEXT("First season's start day is not 1 or last season's doesnt cover the whole year"));
		return false;
	}*/

	return true;
}






void USeasonSubsystem::HandleDayChange(int CurrentDay)
{
	if (!SeasonPrameterInstance)
	{
		PRINT_ERROR(LogTemp, 2.0f, TEXT("Season ParameterCollection or ClockAsset is invalid"));
		return;
	}

	float Alpha = 0.0f, CurveAlpha = 0.0f;
	if (USeasonAsset* Season = GetSeasonAlpha(CurrentDay, Alpha, CurveAlpha))
	{
		PRINT_WARNING(LogTemp, 2.0f, TEXT("Season: %s, Alpha: %f, CurveAlpha: %f"), *Season->SeasonName.ToString(), Alpha, CurveAlpha);

		if (CurrentSeason != Season)
		{
			CurrentSeason = Season;
			OnSeasonChanged.Broadcast(CurrentSeason);
		}

		SeasonPrameterInstance->SetScalarParameterValue(TEXT("SeasonSpecular"), FMath::Lerp(0.0f, Season->MaterialSpecular, CurveAlpha));
		SeasonPrameterInstance->SetScalarParameterValue(TEXT("SeasonRoughness"), FMath::Lerp(0.0f, Season->MaterialRoughness, CurveAlpha));
		SeasonPrameterInstance->SetScalarParameterValue(TEXT("SeasonOpacity"), FMath::Lerp(0.0f, Season->MaterialOpacity, CurveAlpha));
		SeasonPrameterInstance->SetScalarParameterValue(TEXT("SeasonWind"), FMath::Lerp(0.0f, Season->MaterialWind, CurveAlpha));
		SeasonPrameterInstance->SetVectorParameterValue(TEXT("SeasonColor"), FMath::CInterpTo(FLinearColor::Transparent, Season->MaterialColor, CurveAlpha, 1.0f));
	}
}


bool USeasonSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void USeasonSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	Super::OnWorldComponentsUpdated(InWorld);
	LOG_WARNING(LogTemp, TEXT("SeasonSubsystem world components updated"));


	AWorldConfigSettings* WorldSettings = Cast<AWorldConfigSettings>(InWorld.GetWorldSettings());
	if (!IsValid(WorldSettings))
	{
		LOG_ERROR(LogTemp, TEXT("WorldConfigSettings is invalid"));
		return;
	}

	UEnvironmentAsset* EnvironmentAsset = Cast<UEnvironmentAsset>(WorldSettings->EnvironmentAsset);
	if (!IsValid(EnvironmentAsset))
	{
		LOG_ERROR(LogTemp, TEXT("EnvironmentAsset is invalid"));
		return;
	}

	if (!EnvironmentAsset->bEnableSeason)
	{
		LOG_ERROR(LogTemp, TEXT("Season is disabled"));
		return;
	}

	if (!EnvironmentAsset->SeasonMaterialParameter)
	{
		LOG_ERROR(LogTemp, TEXT("SeasonMaterialParameter is invalid"));
		return;
	}

	for (const TObjectPtr<UPrimaryDataAsset>& SeasonAsset : EnvironmentAsset->DefaultSeasons)
	{
		if (USeasonAsset* Season = Cast<USeasonAsset>(SeasonAsset))
		{
			SeasonAssets.Add(Season);
		}
	}

	SeasonPrameterInstance = InWorld.GetParameterCollectionInstance(EnvironmentAsset->SeasonMaterialParameter);
	if (!IsValid(SeasonPrameterInstance))
	{
		LOG_ERROR(LogTemp, TEXT("Season MaterialParameterCollectionInstance is invalid"));
		return;
	}

	UClockAsset* ClockAsset = Cast<UClockAsset>(WorldSettings->ClockAsset);
	if (!IsValid(ClockAsset))
	{
		LOG_ERROR(LogTemp, TEXT("ClockAsset is invalid"));
		return;
	}
	TotalDaysInAYear = ClockAsset->TotalDaysInAYear;

	if (IClockProviderInterface* ClockInterfacePtr = SubsystemUtils::GetSubsystemInterface<UWorld, UWorldSubsystem, IClockProviderInterface>(GetWorld()))
	{
		ClockInterfacePtr->GetOnGameDayChanged().AddUObject(this, &USeasonSubsystem::HandleDayChange);
		ClockInterface = TWeakInterfacePtr<IClockProviderInterface>(ClockInterfacePtr);
	}
}

void USeasonSubsystem::Deinitialize()
{
	if (IClockProviderInterface* ClockInterfacePtr = ClockInterface.Get())
	{
		ClockInterface->GetOnGameDayChanged().RemoveAll(this);
	}
	ClockInterface.Reset();

	SeasonAssets.Empty();

	CurrentSeason = nullptr;
	SeasonPrameterInstance = nullptr;


	LOG_WARNING(LogTemp, TEXT("SeasonSubsystem deinitialized"));
	Super::Deinitialize();
}

