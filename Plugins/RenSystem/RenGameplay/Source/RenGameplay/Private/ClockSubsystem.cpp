// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "ClockSubsystem.h"

// Engine Headers

// Project Headers
#include "RenAsset/Public/Game/ClockAsset.h"

#include "RenCore/Public/Interface/StorageProviderInterface.h"
#include "RenCoreLibrary/Public/SubsystemUtils.h"
#include "RenCoreLibrary/Public/TimerUtils.h"
#include "RenCoreLibrary/Public/LogMacro.h"
#include "RenCore/Public/Record/ClockRecord.h"
#include "RenCore/Public/WorldConfigSettings.h"



bool UClockSubsystem::StartClock()
{
	if (!TimerUtils::StartTimer(ClockTimerHandle, this, &UClockSubsystem::HandleClockTick, 1.0f))
	{
		LOG_ERROR(LogTemp, TEXT("Failed to create timer or timer is already running"));
		return false;
	}

	OnGameClockStarted.Broadcast();
	return true;
}

bool UClockSubsystem::StopClock()
{
	if (TimerUtils::PauseTimer(ClockTimerHandle, this))
	{
		OnGameClockStopped.Broadcast();
		return true;
	}

	LOG_ERROR(LogTemp, TEXT("Clock is not running or does not exist"));
	return false;
}


float UClockSubsystem::GetCurrentTime() const
{
	return FMath::Clamp(CurrentTime, 0.0f, TotalSecondsInADay);
}

float UClockSubsystem::GetNormalizedTime() const
{
	return FMath::Clamp(GetCurrentTime() / TotalSecondsInADay, 0.0f, 1.0f);
}

float UClockSubsystem::GetSimulatedRealTime() const
{
	return FMath::GetMappedRangeValueClamped(TRange<float>(0.0f, TotalSecondsInADay), TRange<float>(0.0f, 24.0f), GetCurrentTime());
}

FString UClockSubsystem::GetFormattedTime(const FString& Format, int BaseYear) const
{
	int TotalSeconds = FMath::FloorToInt(GetCurrentTime());
	int Hours = TotalSeconds / 3600;
	int Minutes = (TotalSeconds % 3600) / 60;
	int Seconds = TotalSeconds % 60;

	FString AmPm = IsDay() ? TEXT("AM") : TEXT("PM");

	FString Result = Format;
	Result = Result.Replace(TEXT("hh"), *FString::Printf(TEXT("%02d"), Hours));
	Result = Result.Replace(TEXT("mm"), *FString::Printf(TEXT("%02d"), Minutes));
	Result = Result.Replace(TEXT("ss"), *FString::Printf(TEXT("%02d"), Seconds));
	Result = Result.Replace(TEXT("dd"), *FString::Printf(TEXT("%02d"), GetCurrentDay()));
	Result = Result.Replace(TEXT("yy"), *FString::Printf(TEXT("%d"), GetCurrentYear() + BaseYear));
	Result = Result.Replace(TEXT("ap"), *AmPm);

	return Result;
}



int UClockSubsystem::GetCurrentDay() const
{
	return FMath::Clamp(CurrentDay, 1, TotalDaysInAYear);
}



bool UClockSubsystem::IsDay() const
{
	float RealHour = GetSimulatedRealTime();
	return RealHour >= 6.0f && RealHour < 18.0f;
}

bool UClockSubsystem::IsNight() const
{
	return !IsDay();
}

int UClockSubsystem::GetCurrentYear() const
{
	return FMath::Max(CurrentYear, 1);
}



float UClockSubsystem::GetSmoothNormalizedTime() const
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

bool UClockSubsystem::IsClockActive() const
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	return TimerManager.IsTimerActive(ClockTimerHandle);
}



void UClockSubsystem::LoadStoredTime()
{
	IClockRecordProviderInterface* ClockRecordInterfacePtr = ClockRecordInterface.Get();
	if (!ClockRecordInterfacePtr)
	{
		LOG_ERROR(LogTemp, TEXT("ClockRecordInterface is not valid"));
		return;
	}

	FName MapName = FName(GetWorld()->GetMapName());
	const TMap<FName, FClockRecord>& Records = ClockRecordInterfacePtr->GetClockRecords();

	if (const FClockRecord* ClockRecord = Records.Find(MapName))
	{
		CurrentTime = FMath::Clamp(ClockRecord->Time, 0.0f, TotalSecondsInADay);
		CurrentDay = FMath::Clamp(ClockRecord->Day, 1, TotalDaysInAYear);
		CurrentYear = FMath::Max(ClockRecord->Year, 1);

		OnGameDayChanged.Broadcast(CurrentDay);
		OnGameYearChanged.Broadcast(CurrentYear);
		OnGameTimeChanged.Broadcast(CurrentTime);

		LOG_INFO(LogTemp, TEXT("Clock day & time loaded"));
	}
	else
	{
		LOG_ERROR(LogTemp, TEXT("Clock day & time not found"));
	}
}

void UClockSubsystem::UpdateStoredTime()
{
	IClockRecordProviderInterface* ClockRecordInterfacePtr = ClockRecordInterface.Get();
	if (!ClockRecordInterfacePtr)
	{
		LOG_ERROR(LogTemp, TEXT("ClockRecordInterface is not valid"));
		return;
	}

	FName MapName = FName(GetWorld()->GetMapName());
	TMap<FName, FClockRecord>& Records = ClockRecordInterfacePtr->GetMutableClockRecords();

	if (FClockRecord* ClockRecord = Records.Find(MapName))
	{
		ClockRecord->Time = GetCurrentTime();
		ClockRecord->Day = GetCurrentDay();
		ClockRecord->Year = GetCurrentYear();

		LOG_INFO(LogTemp, TEXT("Clock day & time updated"));
	}
	else
	{
		Records.Add(MapName, FClockRecord(GetCurrentTime(), GetCurrentDay(), GetCurrentYear()));

		LOG_ERROR(LogTemp, TEXT("Clock day & time added"));
	}
}



void UClockSubsystem::HandleClockTick()
{
	CurrentTime += 1.0f;
	if (CurrentTime >= TotalSecondsInADay)
	{
		CurrentTime = 0.0f;
		CurrentDay++;

		if (CurrentDay > TotalDaysInAYear)
		{
			CurrentDay = 1;
			CurrentYear++;

			OnGameYearChanged.Broadcast(CurrentYear);
		}

		OnGameDayChanged.Broadcast(CurrentDay);
	}

	LastTickAt = GetWorld()->GetTimeSeconds();
	OnGameTimeChanged.Broadcast(CurrentTime);
}

void UClockSubsystem::HandleWorldBeginTearDown(UWorld* World)
{
	UpdateStoredTime();
}


void UClockSubsystem::LoadClockRecord(UWorld& InWorld)
{
	UGameInstance* GameInstance = InWorld.GetGameInstance();
	if (!IsValid(GameInstance))
	{
		LOG_ERROR(LogTemp, TEXT("GameInstance is invalid"));
		return;
	}

	IStorageProviderInterface* StorageInterface = SubsystemUtils::GetSubsystemInterface<UGameInstance, UGameInstanceSubsystem, IStorageProviderInterface>(GameInstance);
	if (!StorageInterface)
	{
		LOG_ERROR(LogTemp, TEXT("StorageInterface is invalid"));
		return;
	}

	UObject* SaveGame = StorageInterface->GetLocalStorage();
	if (IsValid(SaveGame) && SaveGame->Implements<UClockRecordProviderInterface>())
	{
		IClockRecordProviderInterface* ClockRecordInterfacePtr = Cast<IClockRecordProviderInterface>(SaveGame);
		if (ClockRecordInterfacePtr)
		{
			ClockRecordInterface = TWeakInterfacePtr<IClockRecordProviderInterface>(ClockRecordInterfacePtr);
			LoadStoredTime();
		}
		else
		{
			LOG_ERROR(LogTemp, TEXT("ClockRecordInterface cast failed"));
		}
	}
	else
	{
		LOG_ERROR(LogTemp, TEXT("SaveGame is invalid or does not implement ClockRecordProviderInterface"));
	}
}



bool UClockSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UClockSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("ClockSubsystem Initialized"));
}

void UClockSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	Super::OnWorldComponentsUpdated(InWorld);
	LOG_WARNING(LogTemp, TEXT("ClockSubsystem OnWorldComponentsUpdated"));


	FWorldDelegates::OnWorldBeginTearDown.RemoveAll(this);
	FWorldDelegates::OnWorldBeginTearDown.AddUObject(this, &UClockSubsystem::HandleWorldBeginTearDown);

	AWorldConfigSettings* WorldSettings = Cast<AWorldConfigSettings>(InWorld.GetWorldSettings());
	if (!IsValid(WorldSettings))
	{
		LOG_ERROR(LogTemp, TEXT("WorldSettings is invalid"));
		return;
	}

	UClockAsset* LoadedAsset = Cast<UClockAsset>(WorldSettings->ClockAsset);
	if (!IsValid(LoadedAsset))
	{
		LOG_ERROR(LogTemp, TEXT("ClockAsset is invalid"));
		return;
	}

	if (!LoadedAsset->bEnableClock)
	{
		LOG_ERROR(LogTemp, TEXT("Clock is disabled"));
		return;
	}

	TotalSecondsInADay = LoadedAsset->TotalSecondsInADay;
	TotalDaysInAYear = LoadedAsset->TotalDaysInAYear;
	ClockAsset = LoadedAsset;

	LoadClockRecord(InWorld);
	StartClock();
}

void UClockSubsystem::Deinitialize()
{
	FWorldDelegates::OnWorldBeginTearDown.RemoveAll(this);

	TimerUtils::ClearTimer(ClockTimerHandle, this);

	LOG_WARNING(LogTemp, TEXT("ClockSubsystem Deinitialized"));
	Super::Deinitialize();
}

