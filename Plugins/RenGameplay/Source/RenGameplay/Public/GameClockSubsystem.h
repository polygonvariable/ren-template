// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "RenCore/Public/Interface/GameClockInterface.h"

// Generated Headers
#include "GameClockSubsystem.generated.h"

// Forward Declarations
class UTimer;
class UGameClockAsset;


/**
 *
 */
UCLASS()
class RENGAMEPLAY_API UGameClockSubsystem : public UWorldSubsystem, public IGameClockSubsystemInterface
{

	GENERATED_BODY()

public:

	UPROPERTY()
	float TotalSecondsInADay = 60.0f; // Total time of a day in game

	UPROPERTY()
	int TotalDaysInAYear = 30; // Total days in game to complete a year



	UFUNCTION(BlueprintPure)
	UGameClockAsset* GetClockAsset() const;



	UFUNCTION()
	void StartClock();

	UFUNCTION()
	void StopClock();



	UFUNCTION(BlueprintCallable)
	float GetCurrentTime() const;

	UFUNCTION(BlueprintPure)
	FString GetFormattedTime(const FString& Format = TEXT("hh:mm:ss ap")) const;

	UFUNCTION(BlueprintPure)
	float GetNormalizedTime() const;




	UFUNCTION(BlueprintPure)
	float GetSimulatedRealTime() const;



	UFUNCTION(BlueprintCallable)
	int GetCurrentDay() const;

	UFUNCTION(BlueprintPure)
	bool IsDay() const;

	UFUNCTION(BlueprintPure)
	bool IsNight() const;


protected:

	FDelegateHandle OnWorldBeginTearDownHandle;

	TWeakInterfacePtr<IGameClockStorageInterface> GameClockInterface;

	UPROPERTY()
	TObjectPtr<UGameClockAsset> ClockAsset;
	
	UPROPERTY()
	TObjectPtr<UTimer> ClockTimer;


	int CurrentDay = 1; // Clamped from 1 and TotalDaysInAYear
	float CurrentTime = 0.0f; // Clamped from 0 and TotalSecondsInADay
	float LastTickAt = 0.0f;


	void CreateClock();
	void CleanupClock();

	void LoadWorldTime();
	void SaveWorldTime();

	void HandleClockTick(float ElapsedTime);
	void HandleWorldBeginTearDown(UWorld* World);

public:

	float GetSmoothNormalizedTime() const override;
	bool GetIsActive() const override;

	virtual FOnGameDayChanged& GetOnGameDayChanged() override { return OnGameDayChanged; }
	virtual FOnGameTimeChanged& GetOnGameTimeChanged() override { return OnGameTimeChanged; }
	virtual FOnGameClockStarted& GetOnGameClockStarted() override { return OnGameClockStarted; }
	virtual FOnGameClockStopped& GetOnGameClockStopped() override { return OnGameClockStopped; }

protected:

	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldComponentsUpdated(UWorld& InWorld) override;
	virtual void Deinitialize() override;

public:

	UPROPERTY(BlueprintAssignable)
	FOnGameDayChanged OnGameDayChanged;

	UPROPERTY(BlueprintAssignable)
	FOnGameTimeChanged OnGameTimeChanged;

	UPROPERTY(BlueprintAssignable)
	FOnGameClockStarted OnGameClockStarted;

	UPROPERTY(BlueprintAssignable)
	FOnGameClockStopped OnGameClockStopped;

};

