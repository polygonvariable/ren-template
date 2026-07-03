// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Asset/EventflowAsset.h"

// Generated Headers
#include "QuestAsset.generated.h"


/**
 * 
 */
UCLASS(MinimalAPI, BlueprintType)
class UQuestAsset : public UEventflowAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FText Summary;

	UPROPERTY(EditAnywhere, Instanced)
	TArray<TObjectPtr<UEventflowPrimaryTask>> GlobalTasks;

};

