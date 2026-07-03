// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"

// Generated Headers
#include "WorldConfigSettings.generated.h"

// Module Macros
#define REN_API RCORESETTINGS_API

// Forward Declarations
class UPrimaryDataAsset;



/**
 * 
 * 
 * 
 */
UCLASS()
class REN_API AWorldConfigSettings : public AWorldSettings
{

	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = "World Config Settings")
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "/Script/RenAsset.ClockAsset"), Category = "World Config Settings")
	TObjectPtr<UPrimaryDataAsset> ClockAsset;

	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "/Script/RenEnvironment.EnvironmentAsset"), Category = "World Config Settings")
	TObjectPtr<UPrimaryDataAsset> EnvironmentAsset;

};



// Module Macros
#undef REN_API

