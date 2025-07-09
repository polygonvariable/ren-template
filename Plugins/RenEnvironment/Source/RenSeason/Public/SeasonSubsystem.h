// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

// Project Headers
//#include "RenEnvironment/Public/Profile/WeatherProfile.h"

// Generated Headers
#include "SeasonSubsystem.generated.h"

// Forward Declarations
class UMaterialParameterCollectionInstance;

class USeasonAsset;
class UGameClockAsset;
class UEnvironmentAsset;


/**
 *
 */
UCLASS()
class USeasonSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	USeasonAsset* GetCurrentSeason();

	USeasonAsset* GetSeasonAlpha(int CurrentDay, int TotalDays, float& Alpha, float& CurveAlpha) const;

protected:

	UMaterialParameterCollectionInstance* SeasonPrameterInstance;

	UPROPERTY()
	TWeakObjectPtr<UWorldSubsystem> ClockSubsystem;

	UPROPERTY()
	TObjectPtr<USeasonAsset> CurrentSeason;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UGameClockAsset> GameClockAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UEnvironmentAsset> EnvironmentAsset;



	UFUNCTION()
	bool IsSeasonsValid() const;

	UFUNCTION()
	void HandleDayChange(int CurrentDay);

protected:

	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void PostInitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSeasonChanged, USeasonAsset*, SeasonAsset);
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSeasonChanged OnSeasonChanged;

};

