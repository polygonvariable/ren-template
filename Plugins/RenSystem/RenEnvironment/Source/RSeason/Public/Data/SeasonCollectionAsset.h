// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DataAsset.h"

// Generated Headers
#include "SeasonCollectionAsset.generated.h"

// Module Macros
#define REN_API RSEASON_API

// Forward Declarations
class UCurveFloat;
class FObjectPreSaveContext;
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


    REN_API const USeasonAsset* GetSeasonByDay(int InDay, int InYearLength, float& OutAlpha) const;

    // ~ UPrimaryDataAsset
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
    // ~ End of UPrimaryDataAsset

#if WITH_EDITOR
    // ~ UPrimaryDataAsset
    virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
    virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
    // ~ End of UPrimaryDataAsset
#endif

protected:

#if WITH_EDITORONLY_DATA
    UPROPERTY(Transient, VisibleAnywhere, Category = "Debug")
    int DebugCurrentDay = 1;

    UPROPERTY(Transient, VisibleAnywhere, Category = "Debug")
    FString DebugSeasonName = TEXT_EMPTY;

    UPROPERTY(Transient, VisibleAnywhere, Category = "Debug")
    float DebugSeasonWeight = 0.0f;

    UFUNCTION(CallInEditor, Category = "Debug")
    void SimulateSeason() const;
#endif

public:

    static FPrimaryAssetType GetPrimaryAssetType();

};


// Module Macros
#undef REN_API

