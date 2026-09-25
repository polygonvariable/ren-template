// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/SeasonController.h"

// Engine Headers
#include "Materials/MaterialParameterCollectionInstance.h"

// Project Header
#include "ClockManagerInterface.h"
#include "Core/SeasonSettings.h"
#include "Data/SeasonAsset.h"
#include "Data/SeasonCollectionAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "MaterialLibrary.h"
#include "MaterialSurfaceProperty.h"
#include "System/EnvironmentSubsystem.h"
#include "Util/SubsystemUtil.h"


void USeasonController::Initialize(UMaterialParameterCollectionInstance* InMPCInstance)
{
	MPCInstance = InMPCInstance;
	EnvironmentSubsystem = UEnvironmentSubsystem::Get(GetWorld());

	ClockManager = FSubsystemLibrary::GetSubsystemInterface<IClockManagerInterface>(GetWorld());
	if (ClockManager)
	{
		ClockManager->GetYearLength(YearLength);
		ClockManager->OnClockDayChanged().AddUObject(this, &USeasonController::HandleOnDayChanged);
	}
}

void USeasonController::Deinitialize()
{
	if (ClockManager)
	{
		ClockManager->OnClockDayChanged().RemoveAll(this);
	}
	ClockManager = nullptr;

	MPCInstance = nullptr;
	CurrentCollection = nullptr;
	EnvironmentSubsystem = nullptr;
}


bool USeasonController::AddSeasonCollection(USeasonCollectionAsset* SeasonCollection, int Priority)
{
	return AddPriorityItem(SeasonCollection, Priority);
}

bool USeasonController::RemoveSeasonCollection(int Priority)
{
	return RemovePriorityItem(Priority);
}


#if UE_BUILD_DEVELOPMENT
int USeasonController::GetEditorYearLength() const
{
	return YearLength;
}
int USeasonController::GetEditorCurrentDay() const
{
	if (ClockManager)
	{
		return ClockManager->GetCurrentDay();
	}
	return 1;
}
const USeasonCollectionAsset* USeasonController::GetEditorCurrentSeasonCollection() const
{
	return CurrentCollection;
}
const TMap<int, TWeakObjectPtr<UObject>>& USeasonController::GetEditorSeasonCollectionList() const
{
	return _SeasonItems;
}
#endif


void USeasonController::HandleOnDayChanged(int Day)
{
	if (!IsValid(CurrentCollection) || !IsValid(MPCInstance))
	{
		LOG_ERROR(LogSeason, TEXT("Season collection asset, mpc is invalid"));
		return;
	}

	float Alpha = 0.0f;
	const USeasonAsset* Season = CurrentCollection->GetSeasonByDay(Day, YearLength, Alpha);

	if (IsValid(Season))
	{
		const USeasonSettings* Settings = USeasonSettings::Get();

		FMaterialSurfaceProperty ResultSurface;
		FMaterialSurfaceProperty DefaultSurface;

		FMaterialLibrary::LerpSurfaceProperty(DefaultSurface, Season->SurfaceProperty, Alpha, ResultSurface);
		FMaterialLibrary::SetSurfaceProperty(ResultSurface, MPCInstance, Settings->SurfaceTint, Settings->SurfaceSROW, Settings->SurfaceDCMA);
	}
}

TMap<int, TWeakObjectPtr<UObject>>& USeasonController::GetPriorityItems()
{
	return _SeasonItems;
}

void USeasonController::OnPriorityItemChanged(UObject* PreviousItem, UObject* NewItem)
{
	USeasonCollectionAsset* Collection = Cast<USeasonCollectionAsset>(NewItem);
	if (!IsValid(Collection) || Collection == CurrentCollection)
	{
		LOG_ERROR(LogSeason, TEXT("MPC, SeasonAsset is invalid or already active"));
		return;
	}

	CurrentCollection = Collection;

	if (ClockManager)
	{
		HandleOnDayChanged(ClockManager->GetCurrentDay());
	}
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

