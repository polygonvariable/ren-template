// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"
#include "RenEventflow/Public/EventflowNodeData.h"

// Generated Headers
#include "StoryboardAsset.generated.h"

// Forward Declarations
class UQuestGraph;




UCLASS(BlueprintType)
class RENQUEST_API UStoryboardQuestData : public UEventflowNodeData
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FText QuestTitle;

	//UPROPERTY(EditAnywhere)
	//TSoftObjectPtr<UQuestAsset> QuestAsset;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UQuestGraph> QuestGraph;

	UPROPERTY(EditAnywhere)
	TArray<FText> ExtraQuests;

	virtual const TArray<FText>* GetOutputOptions() const;

};



UCLASS(BlueprintType)
class RENQUEST_API UStoryboardGraph : public UEventflowAsset
{

	GENERATED_BODY()

};
