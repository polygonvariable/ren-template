// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "GameClockSubsystem.h"

// Engine Headers
#include "Kismet/GameplayStatics.h"

// Project Headers
#include "RenCore/Public/Developer/GameMetadataSettings.h"
#include "RenCore/Public/Timer/Timer.h"

#include "RenCore/Public/Library/MiscLibrary.h"
#include "RenCore/Public/Macro/LogMacro.h"
#include "RenCore/Public/Record/ClockRecord.h"
#include "RenCore/Public/Storage/StorageInterface.h"

#include "RenAsset/Public/Game/GameClockAsset.h"



void UGameClockSubsystem::CreateClock()
{
	if (IsValid(ClockTimer))
	{
		LOG_WARNING(LogTemp, TEXT("ClockTimer is already valid"));
		return;
	}

	ClockTimer = NewObject<UTimer>(this);
	if (!IsValid(ClockTimer))
	{
		LOG_ERROR(LogTemp, TEXT("Failed to create ClockTimer"));
		return;
	}

	ClockTimer->OnTick.AddDynamic(this, &UGameClockSubsystem::HandleClockTick);

	LOG_INFO(LogTemp, TEXT("ClockTimer created"));
}

void UGameClockSubsystem::CleanupClock()
{
	if (!IsValid(ClockTimer))
	{
		LOG_ERROR(LogTemp, TEXT("ClockTimer is not valid"));
		return;
	}

	ClockTimer->StopTimer(true);
	ClockTimer->OnTick.RemoveAll(this);
	ClockTimer->MarkAsGarbage();

	LOG_INFO(LogTemp, TEXT("ClockTimer removed"));
}



UGameClockAsset* UGameClockSubsystem::GetClockAsset() const
{
	return ClockAsset;
}

void UGameClockSubsystem::StartClock()
{
	if(!IsValid(ClockTimer))
	{
		LOG_ERROR(LogTemp, TEXT("ClockTimer is not valid"));
		return;
	}

	ClockTimer->StartTimer(1.0f, 0);
	OnGameClockStarted.Broadcast();
}

void UGameClockSubsystem::StopClock()
{
	if(!IsValid(ClockTimer))
	{
		LOG_ERROR(LogTemp, TEXT("ClockTimer is not valid"));
		return;
	}

	ClockTimer->StopTimer();
	OnGameClockStopped.Broadcast();
}



float UGameClockSubsystem::GetCurrentTime() const
{
	return FMath::Clamp(CurrentTime, 0.0f, TotalSecondsInADay);
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

float UGameClockSubsystem::GetNormalizedTime() const
{
	return FMath::Clamp(GetCurrentTime() / TotalSecondsInADay, 0.0f, 1.0f);
}

float UGameClockSubsystem::GetSmoothNormalizedTime() const
{
	float CurrentSeconds = GetWorld()->GetTimeSeconds();
	float ElapsedSinceTick = CurrentSeconds - LastTickAt;
	float SmoothTime = GetCurrentTime() + ElapsedSinceTick;

	if (SmoothTime >= TotalSecondsInADay) SmoothTime -= TotalSecondsInADay;

	return FMath::Clamp(SmoothTime / TotalSecondsInADay, 0.0f, 1.0f);
}

float UGameClockSubsystem::GetSimulatedRealTime() const
{
	return FMath::GetMappedRangeValueClamped(TRange<float>(0.0f, TotalSecondsInADay), TRange<float>(0.0f, 24.0f), GetCurrentTime());
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


bool UGameClockSubsystem::GetIsActive() const
{
	return IsValid(ClockTimer) && ClockTimer->IsActive();
}


void UGameClockSubsystem::LoadWorldTime()
{
	if (!GameClockInterface.IsValid())
	{
		LOG_ERROR(LogTemp, TEXT("Storage is not valid"));
		return;
	}

	FName MapName = FName(GetWorld()->GetMapName());
	const TMap<FName, FClockRecord>& Records = GameClockInterface->GetClockRecords();
	// TMap<FName, FClockRecord>& Records = Storage->ClockRecords;

	if (const FClockRecord* ClockRecord = Records.Find(MapName))
	{
		CurrentTime = FMath::Clamp(ClockRecord->Time, 0.0f, TotalSecondsInADay);
		CurrentDay = FMath::Clamp(ClockRecord->DayCount, 1.0f, TotalDaysInAYear);

		OnGameDayChanged.Broadcast(CurrentDay);
		OnGameTimeChanged.Broadcast(CurrentTime);

		LOG_INFO(LogTemp, TEXT("Clock day & time loaded"));
	}
	else {
		LOG_ERROR(LogTemp, TEXT("Clock day & time not found"));
	}
}

void UGameClockSubsystem::SaveWorldTime()
{
	if (!GameClockInterface.IsValid())
	{
		LOG_ERROR(LogTemp, TEXT("Storage is not valid"));
		return;
	}

	FName MapName = FName(GetWorld()->GetMapName());
	TMap<FName, FClockRecord>& Records = GameClockInterface->GetMutableClockRecords();
	// TMap<FName, FClockRecord>& Records = Storage->ClockRecords;

	if (FClockRecord* ClockRecord = Records.Find(MapName))
	{
		ClockRecord->Time = GetCurrentTime();
		ClockRecord->DayCount = GetCurrentDay();

		LOG_INFO(LogTemp, TEXT("Clock day & time updated"));
	}
	else {
		Records.Add(MapName, FClockRecord(GetCurrentTime(), GetCurrentDay()));

		LOG_INFO(LogTemp, TEXT("Clock day & time added"));
	}
}


void UGameClockSubsystem::HandleClockTick(float ElapsedTime)
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

	LastTickAt = GetWorld()->GetTimeSeconds();
	OnGameTimeChanged.Broadcast(CurrentTime);
}

void UGameClockSubsystem::HandleWorldBeginTearDown(UWorld* World)
{
	SaveWorldTime();
}



bool UGameClockSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UGameClockSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("ClockSubsystem initialized"));


	if (const UGameMetadataSettings* GameMetadata = GetDefault<UGameMetadataSettings>())
	{
		if (GameMetadata->ClockAsset.IsNull())
		{
			LOG_ERROR(LogTemp, TEXT("ClockAsset is not valid"));
			return;
		}

		ClockAsset = Cast<UGameClockAsset>(GameMetadata->ClockAsset.LoadSynchronous());
		if (!IsValid(ClockAsset))
		{
			LOG_ERROR(LogTemp, TEXT("ClockAsset cast failed or is not valid"));
			return;
		}

		TotalSecondsInADay = ClockAsset->TotalSecondsInADay;
		TotalDaysInAYear = ClockAsset->TotalDaysInAYear;
	}
}

void UGameClockSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	Super::OnWorldComponentsUpdated(InWorld);
	LOG_WARNING(LogTemp, TEXT("ClockSubsystem OnWorldComponentsUpdated"));


	if (!FWorldDelegates::OnWorldBeginTearDown.IsBoundToObject(this))
	{
		FWorldDelegates::OnWorldBeginTearDown.AddUObject(this, &UGameClockSubsystem::HandleWorldBeginTearDown);
	}


	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (IsValid(GameInstance))
	{
		IStorageSubsystemInterface* StorageInterface = GetSubsystemInterface<UGameInstance, UGameInstanceSubsystem, IStorageSubsystemInterface>(GameInstance);
		if (!StorageInterface)
		{
			LOG_ERROR(LogTemp, TEXT("StorageInterface is not valid"));
			return;
		}

		USaveGame* SaveGame = StorageInterface->IGetLocalStorage();
		if (!IsValid(SaveGame) || !SaveGame->Implements<UGameClockStorageInterface>())
		{
			LOG_ERROR(LogTemp, TEXT("SaveGame is not valid or does not implement GameClockStorageInterface"));
			return;
		}

		IGameClockStorageInterface* ClockInterface = Cast<IGameClockStorageInterface>(SaveGame);
		if (!ClockInterface)
		{
			LOG_ERROR(LogTemp, TEXT("ClockStorageInterface cast failed or is not valid"));
			return;
		}

		GameClockInterface = TWeakInterfacePtr<IGameClockStorageInterface>(ClockInterface);
		LoadWorldTime();

		CreateClock();
		StartClock();
	}


	/*const TArray<UGameInstanceSubsystem*>& Subsystems = GetWorld()->GetGameInstance()->GetSubsystemArray<UGameInstanceSubsystem>();
	for (UGameInstanceSubsystem* Subsystem : Subsystems)
	{
		if (!IsValid(Subsystem) || !Subsystem->Implements<UStorageSubsystemInterface>())
		{
			LOG_WARNING(LogTemp, TEXT("Subsystem is not valid or does not implement StorageSubsystemInterface"));
			continue;
		}

		IStorageSubsystemInterface* StorageInterface = Cast<IStorageSubsystemInterface>(Subsystem);
		if (!StorageInterface)
		{
			LOG_WARNING(LogTemp, TEXT("Subsystem cast failed or is not valid"));
			break;
		}

		USaveGame* SaveGame = StorageInterface->IGetLocalStorage();
		if (!IsValid(SaveGame) || !SaveGame->Implements<UGameClockStorageInterface>())
		{
			LOG_WARNING(LogTemp, TEXT("SaveGame is not valid or does not implement GameClockInterface"));
			break;
		}

		GameClockInterface = TWeakInterfacePtr<IGameClockStorageInterface>(SaveGame);
		LoadWorldTime();

		break;
	}*/


	//if (UStorageSubsystem* StorageSubsystem = GetGameInstanceSubsystem<UStorageSubsystem>(GetWorld()))
	//{
	//	Storage = StorageSubsystem->GetLocalStorage();
	//	LoadWorldTime();
	//}


	// CreateClock();
	// StartClock();
}

void UGameClockSubsystem::Deinitialize()
{
	FWorldDelegates::OnWorldBeginTearDown.RemoveAll(this);

	CleanupClock();

	LOG_WARNING(LogTemp, TEXT("ClockSubsystem deinitialized"));
	Super::Deinitialize();
}

