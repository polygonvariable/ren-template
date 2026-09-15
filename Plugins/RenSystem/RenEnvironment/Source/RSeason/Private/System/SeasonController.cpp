// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/SeasonController.h"

// Engine Headers
#include "Materials/MaterialParameterCollectionInstance.h"

// Project Header
#include "Core/SeasonSettings.h"
#include "Data/SeasonAsset.h"
#include "Data/SeasonCollectionAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "MaterialLibrary.h"
#include "System/EnvironmentSubsystem.h"


void USeasonController::Initialize(UMaterialParameterCollectionInstance* InMPCInstance)
{
	MPCInstance = InMPCInstance;
	EnvironmentSubsystem = UEnvironmentSubsystem::Get(GetWorld());


	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(TimerHandle, this, &USeasonController::HandleOnTimerTick, 0.5f, FTimerManagerTimerParameters{ .bLoop = true, .bMaxOncePerFrame = true });

}

void USeasonController::Deinitialize()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(TimerHandle);
	TimerHandle.Invalidate();

	MPCInstance = nullptr;
	CurrentCollection = nullptr;
	EnvironmentSubsystem = nullptr;
}


bool USeasonController::AddSeason(USeasonCollectionAsset* SeasonCollection, int Priority)
{
	return AddPriorityItem(SeasonCollection, Priority);
}

bool USeasonController::RemoveSeason(int Priority)
{
	return RemovePriorityItem(Priority);
}


#if UE_BUILD_DEVELOPMENT
int USeasonController::GetEditorYearLength() const
{
	return (CurrentCollection) ? CurrentCollection->YearLength : -1;
}
int USeasonController::GetEditorCurrentDay() const
{
	return CurrentDay;
}
const USeasonCollectionAsset* USeasonController::GetEditorCurrentSeasonCollection() const
{
	return CurrentCollection;
}
const TMap<int, TWeakObjectPtr<UObject>>& USeasonController::GetEditorSeasonCollectionList() const
{
	return _PriorityItems;
}
#endif


void USeasonController::HandleOnTimerTick()
{
	if (!CurrentCollection)
	{
		return;
	}

    int YearLength = CurrentCollection->YearLength;
    if (CurrentCollection->Seasons.Num() == 0 || YearLength <= 0)
    {
        return;
    }

    const int Day = (CurrentDay - 1 + YearLength) % YearLength;
    const int StartDay = (CurrentCollection->SeasonStartDay - 1 + YearLength) % YearLength;
    const int RelativeDay = (Day - StartDay + YearLength) % YearLength;

    int AccumulatedDays = 0;
    for (const FSeasonData& Season : CurrentCollection->Seasons)
    {
        const int SeasonStartRelativeDay = AccumulatedDays;
        AccumulatedDays += Season.Duration;

        if (RelativeDay < AccumulatedDays)
        {
            if (!Season.Asset || !Season.Asset->TransitionCurve)
            {
                return;
            }

			const USeasonSettings* Settings = USeasonSettings::Get();

            const int LocalDay = RelativeDay - SeasonStartRelativeDay;
            const float Alpha = (Season.Duration > 1) ? static_cast<float>(LocalDay) / static_cast<float>(Season.Duration - 1) : 1.0f;

            float SeasonWeight = FMath::Clamp(Season.Asset->TransitionCurve->GetFloatValue(Alpha), 0.0f, 1.0f);

			FMaterialSurfaceProperty ResultSurface;
			FMaterialSurfaceProperty DefaultSurface;

			FMaterialLibrary::LerpSurfaceProperty(DefaultSurface, Season.Asset->SurfaceProperty, SeasonWeight, ResultSurface);
			FMaterialLibrary::SetSurfaceProperty(ResultSurface, MPCInstance, Settings->SurfaceTint, Settings->SurfaceSROW, Settings->SurfaceDCMA);
            break;
        }
    }

	CurrentDay++;
	if (CurrentDay > YearLength)
	{
		CurrentDay = 1;
	}
}

TMap<int, TWeakObjectPtr<UObject>>& USeasonController::GetPriorityItems()
{
	return _PriorityItems;
}

int& USeasonController::GetHighestPriority()
{
	return _HighestPriority;
}

void USeasonController::OnPriorityItemChanged(UObject* Item)
{
	USeasonCollectionAsset* Collection = Cast<USeasonCollectionAsset>(Item);
	if (!IsValid(MPCInstance) || !IsValid(Collection) || Collection == CurrentCollection)
	{
		LOG_ERROR(LogSeason, TEXT("MPC, SeasonAsset is invalid or already active"));
		return;
	}

	CurrentCollection = Collection;
}

void USeasonController::OnPriorityItemRemoved(UObject* Item, bool bReplaced)
{
	USeasonCollectionAsset* SeasonAsset = Cast<USeasonCollectionAsset>(Item);
	if (!IsValid(SeasonAsset))
	{
		LOG_ERROR(LogSeason, TEXT("SeasonAsset is invalid"));
		return;
	}
}

void USeasonController::OnPriorityItemCleared()
{
	CurrentCollection = nullptr;

	LOG_ERROR(LogSeason, TEXT("Season controller has no items left, which was not supposed to happen"));
}

