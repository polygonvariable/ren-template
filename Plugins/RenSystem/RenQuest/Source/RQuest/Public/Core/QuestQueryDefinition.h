// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "FilterGroup.h"

// Generated Headers
#include "QuestQueryDefinition.generated.h"

// Forward Declarations
class UQuestEngine;


/**
 *
 */
USTRUCT()
struct FQuestQueryDefinition
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	bool bActive = true;

	UPROPERTY(EditAnywhere)
	FFilterGroup FilterRule;

};

/**
 *
 */
USTRUCT()
struct FQuestQueryResult
{

	GENERATED_BODY()

public:

	UPROPERTY()
	bool bActive = false;

	UPROPERTY()
	bool bTracked = false;

	UPROPERTY()
	FGuid ActiveNodeId;

	UPROPERTY()
    FPrimaryAssetId QuestAssetId;

	UPROPERTY()
	TWeakObjectPtr<UQuestEngine> QuestEngine;

	void Reset()
	{
		bActive = false;
		bTracked = false;
		ActiveNodeId.Invalidate();
		QuestAssetId = FPrimaryAssetId();
		QuestEngine.Reset();
	}

};

