// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "WorldConfigAsset.h"

// Generated Headers
#include "SeasonWorldConfig.generated.h"

// Forward Declarations
class UMaterialParameterCollection;
class USeasonController;


/**
 *
 */
UCLASS(MinimalAPI)
class USeasonWorldConfig : public UWorldConfigAsset
{

    GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly, Category = "Weather")
    bool bEnabled = true;

    UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowedTypes = "Season.Collection"))
    FPrimaryAssetId DefaultSeason;

    UPROPERTY(EditDefaultsOnly, Category = "Default")
    int DefaultPriority = 0;

    UPROPERTY(EditDefaultsOnly, Category = "Material")
    TObjectPtr<UMaterialParameterCollection> SeasonMPC;

    UPROPERTY(EditDefaultsOnly, Category = "Controller")
    TSubclassOf<USeasonController> SeasonController;


#if WITH_EDITOR
    // ~ UPrimaryDataAsset
    virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
    // ~ End of UPrimaryDataAsset
#endif

public:

    static const USeasonWorldConfig* Get(UWorld* World);

};

