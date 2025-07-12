// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

// Generated Headers
#include "ClockAsset.generated.h"


/**
 *
 */
UCLASS()
class RENASSET_API UClockAsset : public UPrimaryDataAsset
{

	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	bool bEnableClock = false;

	/**
	* Total seconds taken to complete a day
	*/
	UPROPERTY(EditAnywhere, Meta = (UIMin = "0", ClampMin = "0"))
	int TotalSecondsInADay = 60;

	/**
	* Total days in game to complete a year
	*/
	UPROPERTY(EditAnywhere, Meta = (UIMin = "1", ClampMin = "1"))
	int TotalDaysInAYear = 30;

};

