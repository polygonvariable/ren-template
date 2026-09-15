// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DataAsset.h"

// Project Headers
#include "MaterialSurfaceProperty.h"

// Generated Headers
#include "SeasonAsset.generated.h"


/**
 *
 */
UCLASS()
class USeasonAsset : public UPrimaryDataAsset
{

    GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly, Category = "Season")
    FName SeasonName = NAME_None;

    UPROPERTY(EditDefaultsOnly, Category = "Season Effect")
    FMaterialSurfaceProperty SurfaceProperty;

    UPROPERTY(EditDefaultsOnly, Category = "Transition")
    TObjectPtr<UCurveFloat> TransitionCurve;


    // ~ UPrimaryDataAsset
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
    // ~ End of UPrimaryDataAsset

#if WITH_EDITOR
    UFUNCTION(Category = "Editor", CallInEditor)
    void EditorApplySeasonToWorld() const;
    void EditorApplySeasonToWorld(float Alpha) const;

    // ~ UPrimaryDataAsset
    virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
    // ~ End of UPrimaryDataAsset
#endif

public:

    static FPrimaryAssetType GetPrimaryAssetType();

};

