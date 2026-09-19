// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Clock/ClockInstance.h"
#include "WorldConfigAsset.h"

// Generated Headers
#include "ClockWorldConfig.generated.h"

// Forward Declaration
class FObjectPreSaveContext;


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

	/**
	* If clock data can be saved and loaded
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Clock")
	bool bIsTransient = false;

	/**
	* Seconds between ticks
	*/
	UPROPERTY(EditAnywhere, Category = "Clock", meta = (UIMin = "0.05", ClampMin = "0.05", UIMax = "10.0", ClampMax = "10.0", Units = "Seconds"))
	float TickInterval = 1;

	/**
	* Total seconds taken to complete a day
	*/
	UPROPERTY(EditAnywhere, Category = "Clock", meta = (UIMin = "1", ClampMin = "1", Units = "Seconds"))
	int DayLength = 60;

	/**
	* Total days in game to complete a year
	*/
	UPROPERTY(EditAnywhere, Category = "Clock", meta = (UIMin = "1", ClampMin = "1"))
	int YearLength = 30;

	UPROPERTY(EditAnywhere, Category = "Default")
	FClockInstance DefaultClock;


#if WITH_EDITOR
	// ~ UPrimaryDataAsset
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
	// ~ End of UPrimaryDataAsset
#endif

};


