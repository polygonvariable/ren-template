// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers
#include "WeatherWeight.generated.h"

// Forward Declarations
class UWeatherAsset;
class UPrimaryDataAsset;



/**
 *
 */
USTRUCT(BlueprintType)
struct FWeightedItem
{

    GENERATED_BODY()

public:

    FGuid ItemID = FGuid::NewGuid();

    UPROPERTY(EditAnywhere)
    int ItemWeight = 0;

    bool operator==(const FWeightedItem& Other) const
    {
        return ItemID == Other.ItemID && ItemWeight == Other.ItemWeight;
    }

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FWeightedWeatherItem : public FWeightedItem
{

    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere)
    TObjectPtr<UWeatherAsset> WeatherAsset;
    
};

FORCEINLINE uint32 GetTypeHash(const FWeightedItem& Item)
{
    return HashCombine(GetTypeHash(Item.ItemID), GetTypeHash(Item.ItemWeight));
}


/**
 *
 */
USTRUCT(BlueprintType)
struct FWeatherCollection
{

    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere)
    TSet<FWeightedWeatherItem> Weathers;

public:

    int GetTotalWeight() const
	{
		int TotalWeight = 0;
		for (const FWeightedItem& Item : Weathers)
		{
			TotalWeight += Item.ItemWeight;
		}
		return TotalWeight;
	}

    UWeatherAsset* GetRandomItem() const
    {
        int TotalWeight = GetTotalWeight();
        float Random = FMath::FRandRange(0.0f, TotalWeight);
        float Cumulative = 0.0f;

        for (const FWeightedWeatherItem& Weather : Weathers)
        {
            Cumulative += Weather.ItemWeight;
            if (Random <= Cumulative)
            {
                return Weather.WeatherAsset;
            }
        }

        return nullptr;
    }

};


