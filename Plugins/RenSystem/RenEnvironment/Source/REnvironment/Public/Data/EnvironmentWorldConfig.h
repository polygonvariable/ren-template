// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

// Project Headers
#include "Data/Asset/MetadataAsset.h"
#include "WorldConfigAsset.h"

// Generated Headers
#include "EnvironmentWorldConfig.generated.h"

// Forward Declarations
class UPriorityList;
class UEnvironmentProfileAsset;
class UEnvironmentStackedController;
class UEnvironmentDiscreteController;


/**
 *
 */
UCLASS(MinimalAPI)
class UEnvironmentWorldConfig : public UWorldConfigAsset
{

    GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly, Category = "Environment")
    bool bEnabled = true;

    UPROPERTY(EditDefaultsOnly, Category = "Controller")
    TArray<TSubclassOf<UEnvironmentStackedController>> StackedControllers;

    UPROPERTY(EditDefaultsOnly, Category = "Controller")
    TArray<TSubclassOf<UEnvironmentDiscreteController>> DiscreteControllers;

    UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowedTypes = "Environment.Profile"))
    TArray<FPrimaryAssetId> DefaultProfiles;

    UPROPERTY(EditDefaultsOnly, Category = "Default")
    int ProfilePriority = 0;

#if WITH_EDITOR
    // ~ UPrimaryDataAsset
    virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
    // ~ End of UPrimaryDataAsset
#endif

};

