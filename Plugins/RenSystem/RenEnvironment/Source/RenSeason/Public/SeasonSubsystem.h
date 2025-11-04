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
class URAssetManager;
class UMaterialParameterCollectionInstance;

class USeasonAsset;
class IClockManagerInterface;



/**
 *
 */
UCLASS()
class USeasonSubsystem : public UWorldSubsystem, public ISeasonProviderInterface
{

	GENERATED_BODY()

public:

	// ~ ISeasonProviderInterface
	virtual UPrimaryDataAsset* GetCurrentSeason() const override;
	virtual FSeasonDelegates& GetSeasonDelegates() override;
	// ~ End of ISeasonProviderInterface

protected:

	FSeasonDelegates Delegates;

	TObjectPtr<URAssetManager> AssetManager;
	UMaterialParameterCollectionInstance* MPC;
	TWeakInterfacePtr<IClockManagerInterface> ClockManagerInterface;

	TObjectPtr<USeasonAsset> CurrentSeason;
	TArray<TObjectPtr<USeasonAsset>> LoadedSeasons;

	void HandleScalarTransition(FName ParameterName, float Target, float Alpha);
	void HandleVectorTransition(FName ParameterName, const FLinearColor& Target, float Alpha);

	USeasonAsset* CalculateSeasonBlend(int CurrentDay, int TotalDays, float& OutAlpha, float& OutCurve) const;

	void LoadDefaultSeasons(const TArray<FPrimaryAssetId>& AssetIds);

	// ~ Bindings
	void HandleDayChanged(int CurrentDay);
	void HandleSeasonsLoaded(const TArray<USeasonAsset*>& Assets);
	// ~ End of Bindings

protected:

	// ~ UWorldSubsystem
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldComponentsUpdated(UWorld& InWorld) override;
	virtual void Deinitialize() override;
	// ~ End of UWorldSubsystem

};

