// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "RCoreClock/Public/ClockDelegates.h"
#include "RCoreClock/Public/ClockManagerInterface.h"

// Generated Headers
#include "ClockSubsystem.generated.h"

// Forward Declarations
class UClockAsset;
class IClockProviderInterface;



/**
 *
 */
UCLASS()
class UClockSubsystem : public UWorldSubsystem, public IClockManagerInterface
{

	GENERATED_BODY()

public:

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
	TWeakInterfacePtr<IClockProviderInterface> ClockProvider;

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

	virtual FClockDelegates& GetClockDelegates() override;

	virtual float GetSmoothNormalizedTime() const override;
	virtual bool IsClockActive() const override;

	virtual int GetTotalSecondsInADay() const override { return TotalSecondsInADay; }
	virtual int GetTotalDaysInAYear() const override { return TotalDaysInAYear; }

protected:

	FClockDelegates ClockDelegates;

	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldComponentsUpdated(UWorld& InWorld) override;
	virtual void Deinitialize() override;

};

