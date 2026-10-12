// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/EventflowTransitionData.h"

// Generated Headers
#include "EventflowIO.generated.h"


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

	UPROPERTY()
	FPrimaryAssetId GroupId = FPrimaryAssetId();

	void Reset()
	{
		EntryType = EEventflowEntryType::Root;
		EntryNodeId = FGuid();
		GroupId = FPrimaryAssetId();
	}

	virtual ~FEventflowEntryData() = default;

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

	UPROPERTY()
	EEventflowGraphTransitionType GraphTransition = EEventflowGraphTransitionType::GraphSuccess;

	void Reset()
	{
		ExitNodeId.Invalidate();
	}

	virtual ~FEventflowReturnData() = default;

};

