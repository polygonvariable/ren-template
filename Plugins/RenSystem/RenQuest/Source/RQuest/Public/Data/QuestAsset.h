// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "EventflowAsset.h"

// Generated Headers
#include "QuestAsset.generated.h"


/**
 * 
 */
UCLASS(MinimalAPI)
class UQuestAsset : public UEventflowAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FText Title;

	UPROPERTY(EditAnywhere)
	FText Summary;

	// ~ UPrimaryDataAsset
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("Quest"), GetFName());
	}
	// ~ End of UPrimaryDataAsset

};

