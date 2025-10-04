// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers
#include "CounterContainer.generated.h"



/**
 *
 */
USTRUCT(BlueprintType)
struct FCounterRecord
{

	GENERATED_BODY()

public:

	UPROPERTY()
	int Value = 0;

	UPROPERTY()
	FDateTime Timestamp = FDateTime::UtcNow();

	FCounterRecord() {}
	FCounterRecord(int InValue) : Value(InValue) {}
	FCounterRecord(int InValue, const FDateTime& InTimestamp) : Value(InValue), Timestamp(InTimestamp) {}

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FCounterContainer
{

	GENERATED_BODY()

public:

	UPROPERTY()
	TMap<FName, FCounterRecord> Counters;

	UPROPERTY()
	FGuid ContainerId;

	FCounterContainer() {}
	FCounterContainer(const FGuid& InContainerId) : ContainerId(InContainerId) {}

	friend inline bool operator == (const FCounterContainer& A, const FCounterContainer& B)
	{
		return A.ContainerId == B.ContainerId;
	}

	friend inline uint32 GetTypeHash(const FCounterContainer& A)
	{
		return GetTypeHash(A.ContainerId.ToString());
	}

};

