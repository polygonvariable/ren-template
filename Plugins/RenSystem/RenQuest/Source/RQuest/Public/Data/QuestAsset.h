// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "EventflowAsset.h"

// Generated Headers
#include "QuestAsset.generated.h"

class UQuestGlobalTask;

/**
 * 
 */
UCLASS(MinimalAPI)
class UQuestAsset : public UEventflowAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Quest Detail")
	FText Title;

	UPROPERTY(EditAnywhere, Category = "Quest Detail")
	FText Summary;

	UPROPERTY(EditAnywhere, Category = "Quest Detail")
	bool bCanCancel = false;

	UPROPERTY(EditAnywhere, Instanced, Category = "Quest Global Task")
	TArray<TObjectPtr<UQuestGlobalTask>> GlobalTasks;

	// ~ UEventflowAsset
	virtual UEventflowGlobalTask* GetGlobalTask(FName TaskName) const override;
	// ~ End of UEventflowAsset

	// ~ UPrimaryDataAsset
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UPrimaryDataAsset

#if WITH_EDITORONLY_DATA
	// ~ UPrimaryDataAsset
	virtual void UpdateAssetBundleData() override;
	// ~ End of UPrimaryDataAsset
#endif

};

