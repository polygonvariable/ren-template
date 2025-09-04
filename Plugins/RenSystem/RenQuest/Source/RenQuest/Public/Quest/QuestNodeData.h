// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEventflow/Public/EventflowNodeData.h"

// Generated Headers
#include "QuestNodeData.generated.h"

// Forward Declarations



/**
 *
 * 
 */
UCLASS(BlueprintType)
class RENQUEST_API UQuestObjectiveNodeData : public UEventflowNodeData
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FGuid ObjectiveId = FGuid::NewGuid();

	UPROPERTY(EditAnywhere)
	FText ObjectiveTitle;

	UPROPERTY(EditAnywhere)
	TArray<FText> ExtraObjectives;

public:

	// ~ UEventflowNodeData
	virtual const TArray<FText>* GetOutputOptions() const override;
	// ~ End of UEventflowNodeData

};

