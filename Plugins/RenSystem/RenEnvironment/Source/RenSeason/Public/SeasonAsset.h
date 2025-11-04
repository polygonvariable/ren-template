// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

// Project Headers
#include "RCoreMaterial/Public/MaterialSurfaceProperty.h"
// #include "RenEnvironment/Public/Profile/WeatherWeight.h"

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

    UPROPERTY(EditDefaultsOnly)
    FName SeasonName = NAME_None;



    UPROPERTY(EditDefaultsOnly, Meta = (UIMin = "1", ClampMin = "1"))
    int StartDay = 1;

	UPROPERTY(EditDefaultsOnly, Meta = (UIMin = "1", ClampMin = "1"))
	int EndDay = 10;


    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UCurveFloat> SeasonProgressCurve = nullptr;


    //UPROPERTY(EditDefaultsOnly)
    FMaterialSurfaceProperty SurfaceProperty;
    
    //UPROPERTY(EditDefaultsOnly)
    //TArray<FWeatherWeight> WeatherWeights;


    // ~ UPrimaryDataAsset
    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId(GetPrimaryAssetType(), GetFName());
    }
    // ~ End of UPrimaryDataAsset

    static FPrimaryAssetType GetPrimaryAssetType()
    {
        return FPrimaryAssetType(TEXT("Environment.Season"));
    }

    static FPrimaryAssetId MakePrimaryAssetId(const FName& AssetName)
    {
        return FPrimaryAssetId(GetPrimaryAssetType(), AssetName);
    }

    static bool IsValid(const FPrimaryAssetId& AssetId)
    {
        return AssetId.PrimaryAssetType == GetPrimaryAssetType();
    }

};

