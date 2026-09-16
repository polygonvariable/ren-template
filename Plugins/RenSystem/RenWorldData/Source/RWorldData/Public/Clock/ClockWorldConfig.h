// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "WorldConfigAsset.h"

// Generated Headers
#include "ClockWorldConfig.generated.h"


/**
 *
 */
UCLASS(MinimalAPI)
class UClockWorldConfig : public UWorldConfigAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Clock")
	bool bEnabled = true;

	UPROPERTY(EditDefaultsOnly, Category = "Clock")
	bool bIsTransient = false;

	/**
	* Seconds between ticks
	*/
	UPROPERTY(EditAnywhere, meta = (UIMin = "0.05", ClampMin = "0.05", UIMax = "10.0", ClampMax = "10.0"))
	float TickInterval = 1;

	/**
	* Total seconds taken to complete a day
	*/
	UPROPERTY(EditAnywhere, meta = (UIMin = "0", ClampMin = "0"))
	int DayLength = 60;

	/**
	* Total days in game to complete a year
	*/
	UPROPERTY(EditAnywhere, meta = (UIMin = "1", ClampMin = "1"))
	int YearLength = 30;


#if WITH_EDITOR
	// ~ UPrimaryDataAsset
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
	// ~ End of UPrimaryDataAsset
#endif

};


