// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"
#include "RenEventflow/Public/EventflowNodeData.h"

// Generated Headers
#include "QuestAsset.generated.h"

// Forward Declarations



UCLASS(BlueprintType)
class RENQUEST_API UQuestObjectiveNodeData : public UEventflowNodeData
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FText ObjectiveTitle;

	UPROPERTY(EditAnywhere)
	TArray<FText> ExtraObjectives;

	virtual const TArray<FText>* GetOutputOptions() const;

};



UCLASS(BlueprintType)
class RENQUEST_API UQuestGraph : public UEventflowAsset
{

	GENERATED_BODY()

};

