// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "Engine/DataAsset.h"

// Generated Headers
#include "WorldConfigSettings.generated.h"



/**
 * 
 */
UCLASS()
class RENCORE_API AWorldConfigSettings : public AWorldSettings
{

	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	bool bEnableClock = true;

	UPROPERTY(EditAnywhere, Meta = (AllowedClasses = "/Script/RenAsset.GameClockAsset"))
	TObjectPtr<UPrimaryDataAsset> ClockAsset;

	UPROPERTY(EditAnywhere, Meta = (AllowedClasses = "/Script/RenEnvironment.EnvironmentAsset"))
	TObjectPtr<UPrimaryDataAsset> EnvironmentAsset;

};
