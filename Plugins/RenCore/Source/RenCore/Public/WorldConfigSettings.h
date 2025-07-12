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
class RENCORE_API AWorldConfigSettings : public AWorldSettings
{

	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Meta = (AllowedClasses = "/Script/RenAsset.ClockAsset"))
	TObjectPtr<UPrimaryDataAsset> ClockAsset;

	UPROPERTY(EditAnywhere, Meta = (AllowedClasses = "/Script/RenEnvironment.EnvironmentAsset"))
	TObjectPtr<UPrimaryDataAsset> EnvironmentAsset;

};

