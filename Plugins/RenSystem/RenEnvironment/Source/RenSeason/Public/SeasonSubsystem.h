// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "RenEnvironment/Public/Interface/SeasonProviderInterface.h"

// Generated Headers
#include "SeasonSubsystem.generated.h"

// Forward Declarations
class UMaterialParameterCollectionInstance;
class USeasonAsset;
class UEnvironmentAsset;
class IClockManagerInterface;



/**
 *
 */
UCLASS()
class USeasonSubsystem : public UWorldSubsystem, public ISeasonProviderInterface
{

	GENERATED_BODY()

public:

	USeasonAsset* GetSeasonAlpha(int CurrentDay, float& OutAlpha, float& OutCurveAlpha) const;

protected:

	FOnSeasonChanged OnSeasonChanged;
	UMaterialParameterCollectionInstance* SeasonPrameterInstance;
	TWeakInterfacePtr<IClockManagerInterface> ClockManager;
	int TotalDaysInAYear = 0;

	UPROPERTY()
	TObjectPtr<USeasonAsset> CurrentSeason;

	UPROPERTY()
	TArray<TObjectPtr<USeasonAsset>> SeasonAssets;


	bool IsSeasonsValid() const;
	void HandleDayChange(int CurrentDay);

public:

	virtual const FName GetSeasonName() const override;
	virtual UPrimaryDataAsset* GetSeasonAsset() override;
	virtual FOnSeasonChanged& GetOnSeasonChanged() override;

protected:

	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void OnWorldComponentsUpdated(UWorld& InWorld) override;
	virtual void Deinitialize() override;

};

