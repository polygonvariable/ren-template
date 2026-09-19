// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Clock/ClockSubsystem.h"

// Project Headers
#include "Clock/ClockStorage.h"
#include "Clock/ClockStorageManager.h"
#include "Clock/ClockWorldConfig.h"
#include "Core/StorageProvider.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Util/SubsystemUtil.h"
#include "WorldFragmentSettings.h"


float UClockSubsystem::GetNormalizedTime() const
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	float ElapsedSinceTick = CurrentTime - LastTickAt;
	float Time = GetCurrentTime() + ElapsedSinceTick;

	if (Time >= DayLength)
	{
		Time -= DayLength;
	}

	return FMath::Clamp(Time / DayLength, 0.0f, 1.0f);
}

bool UClockSubsystem::IsClockActive() const
{
	return GetWorld()->GetTimerManager().IsTimerActive(ClockTimer);
}

bool UClockSubsystem::GetDayLength(int& Length) const
{
	if (!IsValid(ClockConfig))
	{
		return false;
	}
	Length = ClockConfig->DayLength;
	return true;
}

bool UClockSubsystem::GetYearLength(int& Length) const
{
	if (!IsValid(ClockConfig))
	{
		return false;
	}
	Length = ClockConfig->YearLength;
	return true;
}


int UClockSubsystem::GetCurrentTime() const
{
	return ClockInstance.Time;
}

int UClockSubsystem::GetCurrentDay() const
{
	return ClockInstance.Day;
}

int UClockSubsystem::GetCurrentYear() const
{
	return ClockInstance.Year;
}


void UClockSubsystem::CreateClockTimer()
{
	float TickTime = FMath::Clamp(ClockConfig->TickInterval, 0.05f, 10.0f);

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(ClockTimer, this, &UClockSubsystem::HandleOnClockTick, TickTime, FTimerManagerTimerParameters{ .bLoop = true, .bMaxOncePerFrame = true });
}

void UClockSubsystem::RemoveClockTimer()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(ClockTimer);
	ClockTimer.Invalidate();
}


void UClockSubsystem::HandleOnClockTick()
{
	int PreviousDay = ClockInstance.Day;

	ClockInstance.AddSeconds(1, DayLength, YearLength);

	if (PreviousDay != ClockInstance.Day)
	{
		ClockDayChanged.Broadcast(ClockInstance.Day);
	}

	LastTickAt = GetWorld()->GetTimeSeconds();
	ClockTimeChanged.Broadcast(ClockInstance.Time);
}

void UClockSubsystem::HandleOnStorageLoaded(UObject* InManager)
{
	UClockStorageManager* StorageManager = Cast<UClockStorageManager>(InManager);
	if (IsValid(StorageManager))
	{
		FClockInstance Instance = StorageManager->GetClockInstance(GetWorld()->GetFName());
		if (Instance > ClockInstance)
		{
			ClockInstance = Instance;
		}

		ClockInstance.Clamp(DayLength, YearLength);
		ClockStorageManager = TWeakObjectPtr<UClockStorageManager>(StorageManager);

		CreateClockTimer();
	}
}


bool UClockSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

bool UClockSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Super::ShouldCreateSubsystem(Outer))
	{
		return false;
	}
	const UClockWorldConfig* Config = AWorldFragmentSettings::GetConfigByClass<UClockWorldConfig>(Cast<UWorld>(Outer));
	return IsValid(Config) && Config->bEnabled;
}

void UClockSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogClock, TEXT("ClockSubsystem Initialized"));
}

void UClockSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	LOG_WARNING(LogClock, TEXT("ClockSubsystem OnWorldComponentsUpdated"));

	ClockConfig = AWorldFragmentSettings::GetConfigByClass<UClockWorldConfig>(&InWorld);
	check(ClockConfig);
	check(ClockConfig->bEnabled);

	DayLength = ClockConfig->DayLength;
	YearLength = ClockConfig->YearLength;

	ClockInstance = ClockConfig->DefaultClock;
	ClockInstance.Clamp(DayLength, YearLength);

	if (!ClockConfig->bIsTransient)
	{
		IStorageProvider* StorageProvider = FSubsystemLibrary::GetSubsystemInterface<IStorageProvider>(InWorld.GetGameInstance());
		if (StorageProvider)
		{
			FStorageDefinition Definition;
			Definition.StorageId = GetFName();
			Definition.StorageClass = UClockStorage::StaticClass();
			Definition.ManagerClass = UClockStorageManager::StaticClass();

			StorageProvider->LoadStorage(Definition, FOnStorageLoaded::CreateUObject(this, &UClockSubsystem::HandleOnStorageLoaded));
		}
	}
	else
	{
		CreateClockTimer();
	}
}

void UClockSubsystem::OnWorldEndPlay(UWorld& InWorld)
{
	UClockStorageManager* StorageManager = ClockStorageManager.Get();
	if (IsValid(StorageManager))
	{
		ClockInstance.Clamp(DayLength, YearLength);
		StorageManager->SetClockInstance(GetWorld()->GetFName(), ClockInstance);
	}
	ClockStorageManager.Reset();

	ClockConfig = nullptr;
	RemoveClockTimer();

	LOG_WARNING(LogClock, TEXT("ClockSubsystem OnWorldEndPlay"));
}

UClockSubsystem* UClockSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return World->GetSubsystem<UClockSubsystem>();
}

