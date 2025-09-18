// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Generated Headers
#include "ClockRecord.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct FClockRecord
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clock Record")
	float Time = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clock Record")
	int Day = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clock Record")
	int Year = 1;

};

