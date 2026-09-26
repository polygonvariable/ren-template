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
USTRUCT(BlueprintType)
struct FEventflowEntry
{
	GENERATED_BODY()

public:

	FEventflowEntry() {};
	FEventflowEntry(EEventflowEntryType InEntryType, FGuid InNodeId) : EntryType(InEntryType), NodeId(InNodeId) {};


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEventflowEntryType EntryType = EEventflowEntryType::Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "EntryType == EEventflowEntryType::Custom", EditConditionHides))
	FGuid NodeId = FGuid();

	void Reset()
    {
        EntryType = EEventflowEntryType::Root;
        NodeId = FGuid();
    }

};

