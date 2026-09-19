// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DataAsset.h"

// Project Headers
#include "MaterialSurfaceProperty.h"

// Generated Headers
#include "WeatherAsset.generated.h"

// Forward Declarations
class FObjectPreSaveContext;
class AWeatherEffectActor;
class UNiagaraSystem;
class UEnvironmentProfileAsset;


/**
 *
 */
UCLASS(MinimalAPI)
class UWeatherAsset : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly, Category = "Weather Details")
    FName WeatherName = NAME_None;

    UPROPERTY(EditDefaultsOnly, Category = "Weather Effect")
    FMaterialSurfaceProperty SurfaceProperty;

    UPROPERTY(VisibleDefaultsOnly, Category = "Weather Effect")
    TArray<FSoftObjectPath> WeatherEffects;

    UPROPERTY(EditDefaultsOnly, Category = "Environment")
    TArray<TObjectPtr<UEnvironmentProfileAsset>> EnvironmentProfiles;

    UPROPERTY(EditDefaultsOnly, Category = "Environment")
    int ProfilePriority = 0;

    UPROPERTY(EditDefaultsOnly, Category = "Transition", meta = (UIMin = "0.05", ClampMin = "0.05", UIMax = "30", ClampMax = "30"))
    float TransitionRate = 0.2f;

    UPROPERTY(EditDefaultsOnly, Category = "Transition", meta = (UIMin = "0.05", ClampMin = "0.05", UIMax = "30", ClampMax = "30"))
    float TransitionDuration = 4.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Transition")
    TObjectPtr<UCurveFloat> TransitionCurve;


    // ~ UPrimaryDataAsset
    virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
    // ~ End of UPrimaryDataAsset

#if WITH_EDITOR
    // ~ UPrimaryDataAsset
    virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
    // ~ End of UPrimaryDataAsset
#endif

protected:

#if WITH_EDITORONLY_DATA
    UPROPERTY(EditDefaultsOnly, Category = "Weather Effect")
    TArray<TSoftObjectPtr<UNiagaraSystem>> NiagaraAssets;

    UPROPERTY(EditDefaultsOnly, Category = "Weather Effect")
    TArray<TSoftObjectPtr<USoundBase>> SoundAssets;

    UFUNCTION(Category = "Debug", CallInEditor)
    void ApplySurfaceToWorld();
#endif

public:

    static FPrimaryAssetType GetPrimaryAssetType();

};

