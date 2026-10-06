// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "EventflowEntry.generated.h"


/**
 *
 */
UENUM(BlueprintType)
enum class EEventflowEntryType : uint8
{
	Root UMETA(DisplayName = "Root"),
	Custom UMETA(DisplayName = "Custom")
};

/**
 *
 */
USTRUCT()
struct FEventflowEntryData
{
	GENERATED_BODY()

public:

	FEventflowEntryData() {};

	UPROPERTY()
	EEventflowEntryType EntryType = EEventflowEntryType::Root;

	UPROPERTY()
	FGuid EntryNodeId = FGuid();

	void Reset()
	{
		EntryType = EEventflowEntryType::Root;
		EntryNodeId = FGuid();
	}

};

/**
 *
 */
USTRUCT()
struct FEventflowReturnData
{

	GENERATED_BODY()

public:

	FEventflowReturnData() {};

	UPROPERTY()
	FGuid ExitNodeId = FGuid();

	void Reset()
	{
		ExitNodeId.Invalidate();
	}

};

