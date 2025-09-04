// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"

// Generated Headers
#include "QuestAsset.generated.h"

// Forward Declarations



/**
 *
 *
 */
UCLASS(BlueprintType, MinimalAPI)
class UQuestAsset : public UEventflowAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGuid QuestId = FGuid::NewGuid();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText QuestTitle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText QuestDescription;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// bool bIsNetworkReplicated = false;

};

