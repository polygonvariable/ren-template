// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers
#include "QuestRecord.generated.h"



/**
 *
 */
USTRUCT(BlueprintType)
struct FQuestRecord
{

	GENERATED_BODY()

public:

	UPROPERTY()
	TArray<FGuid> ActiveObjectives;

	UPROPERTY()
	TArray<FGuid> CompletedObjectives;
	
};

