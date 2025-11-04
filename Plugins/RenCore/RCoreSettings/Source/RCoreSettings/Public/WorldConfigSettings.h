// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"

// Generated Headers
#include "WorldConfigSettings.generated.h"

// Forward Declarations
class UPrimaryDataAsset;



/**
 * 
 */
UCLASS()
class RCORESETTINGS_API AWorldConfigSettings : public AWorldSettings
{

	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Meta = (AllowedClasses = "/Script/RenAsset.ClockAsset"), Category = "World Config Settings")
	TObjectPtr<UPrimaryDataAsset> ClockAsset;

	UPROPERTY(EditAnywhere, Meta = (AllowedClasses = "/Script/RenEnvironment.EnvironmentAsset"), Category = "World Config Settings")
	TObjectPtr<UPrimaryDataAsset> EnvironmentAsset;

	UPROPERTY(EditAnywhere, Category = "World Config Settings")
	FName EnvironmentTag = NAME_None;

};

