// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "CraftInstance.generated.h"


/**
 *
 */
USTRUCT(BlueprintType)
struct FCraftInstance
{

	GENERATED_BODY()

public:

	FCraftInstance() {}
	FCraftInstance(int InPendingQuantity, FDateTime InBatchStartTimestamp, FTimespan InBatchProcessingTime)
	{
		PendingQuantity = InPendingQuantity;
		BatchStartTimestamp = InBatchStartTimestamp;
		BatchProcessingTime = InBatchProcessingTime;
	}


	UPROPERTY(SaveGame)
	int PendingQuantity = 0;

	UPROPERTY(SaveGame)
	FDateTime BatchStartTimestamp;

	UPROPERTY(SaveGame)
	FTimespan BatchProcessingTime;


	bool IsValid() const
	{
		return PendingQuantity > 0 && BatchProcessingTime.GetTicks() > 0;
	}

	void Reset()
	{
		PendingQuantity = 0;
		BatchStartTimestamp = FDateTime::Now();
		BatchProcessingTime = FTimespan::Zero();
	}

	void Sanitize()
	{
		PendingQuantity = FMath::Max(0, PendingQuantity);
	}

	int GetCompletedQuantity() const
	{
		if (BatchProcessingTime.GetTicks() <= 0)
		{
			return 0;
		}

		FTimespan Elapsed = FDateTime::Now() - BatchStartTimestamp;
		int Completed = Elapsed.GetTicks() / BatchProcessingTime.GetTicks();

		return FMath::Clamp(Completed, 0, PendingQuantity);
	}

	FTimespan GetRemainingTime() const
	{
		FDateTime EndTime = BatchStartTimestamp + (BatchProcessingTime * PendingQuantity);
		FTimespan Result = EndTime - FDateTime::Now();

		if (Result < FTimespan::Zero())
		{
			Result = FTimespan::Zero();
		}

		return Result;
	}

};

