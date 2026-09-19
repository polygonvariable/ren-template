// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "Clock/ClockInstance.h"
#include "ClockManagerInterface.h"

// Generated Headers
#include "ClockSubsystem.generated.h"

// Forward Declaration
class UClockWorldConfig;
class UClockStorageManager;


/**
 *
 */
UCLASS(MinimalAPI)
class UClockSubsystem : public UWorldSubsystem, public IClockManagerInterface
{

	GENERATED_BODY()

public:

	// ~ IClockManagerInterface
	virtual float GetNormalizedTime() const override;
	virtual bool IsClockActive() const override;
	virtual bool GetDayLength(int& Length) const override;
	virtual bool GetYearLength(int& Length) const override;

	RWORLDDATA_API virtual int GetCurrentTime() const override;
	RWORLDDATA_API virtual int GetCurrentDay() const override;
	RWORLDDATA_API virtual int GetCurrentYear() const override;
	// ~ End of IClockManagerInterface

	// ~ UWorldSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldComponentsUpdated(UWorld& InWorld) override;
	virtual void OnWorldEndPlay(UWorld& InWorld) override;
	// ~ End of UWorldSubsystem

protected:

	UPROPERTY()
	TWeakObjectPtr<UClockStorageManager> ClockStorageManager;

	UPROPERTY()
	TObjectPtr<const UClockWorldConfig> ClockConfig;

	int DayLength = 1;
	int YearLength = 1;

	FTimerHandle ClockTimer;
	FClockInstance ClockInstance;

	float LastTickAt = 0;


	void CreateClockTimer();
	void RemoveClockTimer();

	// ~ Binding
	void HandleOnClockTick();
	void HandleOnStorageLoaded(UObject* InManager);
	// ~ End of Binding

	// ~ UWorldSubsystem
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	// ~ End of UWorldSubsystem

public:

	static RWORLDDATA_API UClockSubsystem* Get(UWorld* World);

};

