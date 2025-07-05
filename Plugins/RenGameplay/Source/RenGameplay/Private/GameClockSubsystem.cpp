// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "GameClockSubsystem.h"

// Engine Headers
#include "Kismet/GameplayStatics.h"

// Project Headers
#include "RenCore/Public/Developer/GameMetadataSettings.h"
#include "RenCore/Public/Library/MiscLibrary.h"
#include "RenCore/Public/Macro/LogMacro.h"
#include "RenCore/Public/Record/ClockRecord.h"
#include "RenCore/Public/Storage/StorageInterface.h"

#include "RenAsset/Public/Game/GameClockAsset.h"



bool UGameClockSubsystem::StartClock()
{
	if (!TimerUtils::StartTimer(ClockTimerHandle, this, &UGameClockSubsystem::HandleClockTick, 1.0f))
	{
		LOG_ERROR(LogTemp, TEXT("Failed to create timer or timer is already running"));
		return false;
	}

	OnGameClockStarted.Broadcast();
	return true;
}

bool UGameClockSubsystem::StopClock()
{
	if (TimerUtils::PauseTimer(ClockTimerHandle, this))
	{
		OnGameClockStopped.Broadcast();
		return true;
	}

	LOG_ERROR(LogTemp, TEXT("Clock is not running or does not exist"));
	return false;
}



UGameClockAsset* UGameClockSubsystem::GetClockAsset() const
{
	return ClockAsset;
}



float UGameClockSubsystem::GetCurrentTime() const
{
	return FMath::Clamp(CurrentTime, 0.0f, TotalSecondsInADay);
}

float UGameClockSubsystem::GetNormalizedTime() const
{
	return FMath::Clamp(GetCurrentTime() / TotalSecondsInADay, 0.0f, 1.0f);
}

float UGameClockSubsystem::GetSimulatedRealTime() const
{
	return FMath::GetMappedRangeValueClamped(TRange<float>(0.0f, TotalSecondsInADay), TRange<float>(0.0f, 24.0f), GetCurrentTime());
}

FString UGameClockSubsystem::GetFormattedTime(const FString& Format) const
{
	int32 TotalSeconds = FMath::FloorToInt(GetCurrentTime());
	int32 Hours = TotalSeconds / 3600;
	int32 Minutes = (TotalSeconds % 3600) / 60;
	int32 Seconds = TotalSeconds % 60;

	FString AmPm = IsDay() ? TEXT("AM") : TEXT("PM");

	FString Result = Format;
	Result = Result.Replace(TEXT("hh"), *FString::Printf(TEXT("%02d"), Hours));
	Result = Result.Replace(TEXT("mm"), *FString::Printf(TEXT("%02d"), Minutes));
	Result = Result.Replace(TEXT("ss"), *FString::Printf(TEXT("%02d"), Seconds));
	Result = Result.Replace(TEXT("dd"), *FString::Printf(TEXT("%02d"), GetCurrentDay()));
	Result = Result.Replace(TEXT("ap"), *AmPm);

	return Result;
}



int UGameClockSubsystem::GetCurrentDay() const
{
	return FMath::Clamp(CurrentDay, 1, TotalDaysInAYear);
}



bool UGameClockSubsystem::IsDay() const
{
	float RealHour = GetSimulatedRealTime();
	return RealHour >= 6.0f && RealHour < 18.0f;
}

bool UGameClockSubsystem::IsNight() const
{
	return !IsDay();
}



float UGameClockSubsystem::GetSmoothNormalizedTime() const
{
	float CurrentSeconds = GetWorld()->GetTimeSeconds();
	float ElapsedSinceTick = CurrentSeconds - LastTickAt;
	float SmoothTime = GetCurrentTime() + ElapsedSinceTick;

	if (SmoothTime >= TotalSecondsInADay)
	{
		SmoothTime -= TotalSecondsInADay;
	}

	return FMath::Clamp(SmoothTime / TotalSecondsInADay, 0.0f, 1.0f);
}

bool UGameClockSubsystem::IsClockActive() const
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	return TimerManager.IsTimerActive(ClockTimerHandle);
}



void UGameClockSubsystem::LoadStoredTime()
{
	if (!ClockStorageInterface.IsValid())
	{
		LOG_ERROR(LogTemp, TEXT("ClockStorage is not valid"));
		return;
	}

	FName MapName = FName(GetWorld()->GetMapName());
	const TMap<FName, FClockRecord>& Records = ClockStorageInterface->GetClockRecords();

	if (const FClockRecord* ClockRecord = Records.Find(MapName))
	{
		CurrentTime = FMath::Clamp(ClockRecord->Time, 0.0f, TotalSecondsInADay);
		CurrentDay = FMath::Clamp(ClockRecord->Day, 1.0f, TotalDaysInAYear);

		OnGameDayChanged.Broadcast(CurrentDay);
		OnGameTimeChanged.Broadcast(CurrentTime);

		LOG_INFO(LogTemp, TEXT("Clock day & time loaded"));
	}
	else
	{
		LOG_ERROR(LogTemp, TEXT("Clock day & time not found"));
	}
}

void UGameClockSubsystem::UpdateStoredTime()
{
	if (!ClockStorageInterface.IsValid())
	{
		LOG_ERROR(LogTemp, TEXT("ClockStorage is not valid"));
		return;
	}

	FName MapName = FName(GetWorld()->GetMapName());
	TMap<FName, FClockRecord>& Records = ClockStorageInterface->GetMutableClockRecords();

	if (FClockRecord* ClockRecord = Records.Find(MapName))
	{
		ClockRecord->Time = GetCurrentTime();
		ClockRecord->Day = GetCurrentDay();

		LOG_INFO(LogTemp, TEXT("Clock day & time updated"));
	}
	else
	{
		Records.Add(MapName, FClockRecord(GetCurrentTime(), GetCurrentDay()));

		LOG_ERROR(LogTemp, TEXT("Clock day & time added"));
	}
}



void UGameClockSubsystem::HandleClockTick()
{
	CurrentTime += 1.0f;
	if (CurrentTime >= TotalSecondsInADay)
	{
		CurrentTime = 0.0f;
		CurrentDay++;

		if (CurrentDay > TotalDaysInAYear)
		{
			CurrentDay = 1;
		}

		OnGameDayChanged.Broadcast(CurrentDay);
	}

	PRINT_INFO(LogTemp, 1.0f, TEXT("Current time: %f"), CurrentTime);

	LastTickAt = GetWorld()->GetTimeSeconds();
	OnGameTimeChanged.Broadcast(CurrentTime);
}

void UGameClockSubsystem::HandleWorldBeginTearDown(UWorld* World)
{
	UpdateStoredTime();
}



bool UGameClockSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UGameClockSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("ClockSubsystem initialized"));

	const UGameMetadataSettings* GameMetadata = GetDefault<UGameMetadataSettings>();
	if (IsValid(GameMetadata))
	{
		UDataAsset* LoadedAsset = GameMetadata->ClockAsset.LoadSynchronous();
		if (IsValid(LoadedAsset))
		{
			UGameClockAsset* LoadedClockAsset = Cast<UGameClockAsset>(LoadedAsset);
			if (IsValid(LoadedClockAsset))
			{
				ClockAsset = LoadedClockAsset;

				TotalSecondsInADay = LoadedClockAsset->TotalSecondsInADay;
				TotalDaysInAYear = LoadedClockAsset->TotalDaysInAYear;
			}
			else
			{
				LOG_ERROR(LogTemp, TEXT("Loaded ClockAsset is not of type UGameClockAsset"));
			}
		}
		else
		{
			LOG_ERROR(LogTemp, TEXT("Failed to load ClockAsset"));
		}
	}
}

void UGameClockSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	Super::OnWorldComponentsUpdated(InWorld);
	LOG_WARNING(LogTemp, TEXT("ClockSubsystem OnWorldComponentsUpdated"));

	FWorldDelegates::OnWorldBeginTearDown.RemoveAll(this);
	FWorldDelegates::OnWorldBeginTearDown.AddUObject(this, &UGameClockSubsystem::HandleWorldBeginTearDown);

	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (IsValid(GameInstance))
	{
		IStorageSubsystemInterface* StorageInterface = SubsystemUtils::GetSubsystemInterface<UGameInstance, UGameInstanceSubsystem, IStorageSubsystemInterface>(GameInstance);
		if (StorageInterface)
		{
			USaveGame* SaveGame = StorageInterface->IGetLocalStorage();
			if (IsValid(SaveGame) && SaveGame->Implements<UGameClockStorageInterface>())
			{
				IGameClockStorageInterface* ClockInterface = Cast<IGameClockStorageInterface>(SaveGame);
				if (ClockInterface)
				{
					ClockStorageInterface = TWeakInterfacePtr<IGameClockStorageInterface>(ClockInterface);
					LoadStoredTime();
				}
			}
			else
			{
				LOG_ERROR(LogTemp, TEXT("ClockStorage is not valid"));
			}
		}
		else
		{
			LOG_ERROR(LogTemp, TEXT("StorageInterface is not valid"));
		}
	}

	StartClock();
}

void UGameClockSubsystem::Deinitialize()
{
	FWorldDelegates::OnWorldBeginTearDown.RemoveAll(this);

	TimerUtils::ClearTimer(ClockTimerHandle, this);

	LOG_WARNING(LogTemp, TEXT("ClockSubsystem deinitialized"));
	Super::Deinitialize();
}

