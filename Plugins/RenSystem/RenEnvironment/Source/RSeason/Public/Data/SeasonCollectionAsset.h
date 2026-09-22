// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DataAsset.h"

// Generated Headers
#include "SeasonCollectionAsset.generated.h"

// Forward Declarations
class UCurveFloat;
class USeasonAsset;


/**
 *
 */
USTRUCT()
struct FSeasonData
{

    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere)
    TObjectPtr<USeasonAsset> Asset;

    UPROPERTY(EditAnywhere)
    int Duration = 10;

};


/**
 *
 */
UCLASS(MinimalAPI)
class USeasonCollectionAsset : public UPrimaryDataAsset
{

    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere)
    int SeasonStartDay = 1;

    UPROPERTY(EditAnywhere)
    TArray<FSeasonData> Seasons;


    RSEASON_API const USeasonAsset* GetSeasonByDay(int InDay, int InYearLength, float& OutAlpha) const;

    // ~ UPrimaryDataAsset
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
    // ~ End of UPrimaryDataAsset

#if WITH_EDITOR
    // ~ UPrimaryDataAsset
    virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
    // ~ End of UPrimaryDataAsset
#endif

public:

    static FPrimaryAssetType GetPrimaryAssetType();

};

