// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers

// Generated Headers
#include "CraftData.generated.h"

// Module Macros
#define REN_API RCRAFT_API



/**
 *
 */
USTRUCT(BlueprintType)
struct FCraftData
{

	GENERATED_BODY()

public:

	FCraftData() {}
	FCraftData(int InPendingQuantity, FDateTime InBatchStartTimestamp, FTimespan InBatchProcessingTime);


	UPROPERTY(SaveGame)
	int PendingQuantity = 0;

	UPROPERTY(SaveGame)
	FDateTime BatchStartTimestamp;

	UPROPERTY(SaveGame)
	FTimespan BatchProcessingTime;


	REN_API bool IsValid() const;
	REN_API void Reset();
	REN_API void Sanitize();

	REN_API int GetCompletedQuantity() const;
	REN_API FTimespan GetRemainingTime() const;

};



// Module Macros
#undef REN_API

