// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/WorldSettings.h"

// Generated Headers
#include "WorldFragmentSettings.generated.h"

// Module Macros
#define REN_API RCORESETTINGS_API

// Forward Declarations
class UPrimaryDataAsset;
class UWorldFragment;
class UWorldConfigAsset;


/**
 * 
 */
UCLASS(MinimalAPI)
class AWorldFragmentSettings : public AWorldSettings
{

	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = "World Config Settings")
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "/Script/RenAsset.ClockAsset"), Category = "World Config Settings")
	TObjectPtr<UPrimaryDataAsset> ClockAsset;

	UPROPERTY(EditDefaultsOnly, Category = "World Configs")
	TArray<TObjectPtr<UWorldConfigAsset>> Configs;


	REN_API const UWorldConfigAsset* FindConfigByClass(TSubclassOf<UWorldConfigAsset> InClass) const;

	template<typename T>
	const T* FindConfigByClass() const
	{
		return Cast<T>(FindConfigByClass(T::StaticClass()));
	}
	
};


// Module Macros
#undef REN_API

