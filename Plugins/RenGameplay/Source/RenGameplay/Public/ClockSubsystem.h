// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "RenCore/Public/Interface/ClockProviderInterface.h"

// Generated Headers
#include "ClockSubsystem.generated.h"

// Forward Declarations
class UClockAsset;



/**
 *
 */
UCLASS()
class UClockSubsystem : public UWorldSubsystem, public IClockProviderInterface
{

	GENERATED_BODY()

public:

	FOnGameDayChanged OnGameDayChanged;
	FOnGameYearChanged OnGameYearChanged;
	FOnGameTimeChanged OnGameTimeChanged;
	FOnGameClockStarted OnGameClockStarted;
	FOnGameClockStopped OnGameClockStopped;

	float TotalSecondsInADay = 60.0f;
	int TotalDaysInAYear = 30;


	bool StartClock();
	bool StopClock();

	float GetCurrentTime() const;
	float GetNormalizedTime() const;
	float GetSimulatedRealTime() const;
	FString GetFormattedTime(const FString& Format = TEXT("hh:mm:ss ap"), int BaseYear = 2000) const;

	int GetCurrentDay() const;
	bool IsDay() const;
	bool IsNight() const;

	int GetCurrentYear() const;

protected:

	FDelegateHandle OnWorldBeginTearDownHandle;
	TWeakInterfacePtr<IClockRecordProviderInterface> ClockRecordInterface;

	UPROPERTY()
	TObjectPtr<UClockAsset> ClockAsset;
	
	UPROPERTY()
	FTimerHandle ClockTimerHandle;


	/**
	 * Clamped from 1 to TotalDaysInAYear
	 */
	int CurrentDay = 1;

	/**
	 * Clamped from 1
	 */
	int CurrentYear = 1;

	/**
	 * Clamped from 0 to TotalSecondsInADay
	 */
	float CurrentTime = 0.0f;
	float LastTickAt = 0.0f;


	void LoadStoredTime();
	void UpdateStoredTime();

	void HandleClockTick();
	void HandleWorldBeginTearDown(UWorld* World);

	void LoadClockRecord(UWorld& InWorld);

public:

	float GetSmoothNormalizedTime() const override;
	bool IsClockActive() const override;

	virtual FOnGameDayChanged& GetOnGameDayChanged() override { return OnGameDayChanged; }
	virtual FOnGameYearChanged& GetOnGameYearChanged() override { return OnGameYearChanged; }
	virtual FOnGameTimeChanged& GetOnGameTimeChanged() override { return OnGameTimeChanged; }
	virtual FOnGameClockStarted& GetOnGameClockStarted() override { return OnGameClockStarted; }
	virtual FOnGameClockStopped& GetOnGameClockStopped() override { return OnGameClockStopped; }

protected:

	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldComponentsUpdated(UWorld& InWorld) override;
	virtual void Deinitialize() override;

};

