// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "EventflowAsset.h"
#include "Core/QuestType.h"

// Generated Headers
#include "QuestAsset.generated.h"

// Forward Declaration
class UQuestGlobalTask;



/**
 * 
 */
UCLASS(MinimalAPI)
class UQuestAsset : public UEventflowAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Quest Detail", AssetRegistrySearchable)
	FText Title;

	UPROPERTY(EditAnywhere, Category = "Quest Detail", AssetRegistrySearchable)
	FText Summary;

	UPROPERTY(EditAnywhere, Category = "Quest Detail", AssetRegistrySearchable)
	EQuestType QuestType = EQuestType::Story;

	UPROPERTY(EditAnywhere, Category = "Quest Detail")
	bool bPushInHUD = true;

	UPROPERTY(EditAnywhere, Category = "Quest Detail")
	bool bAutoFocus = false;

	UPROPERTY(EditAnywhere, Instanced, Category = "Global Task")
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

public:

	static RQUEST_API FPrimaryAssetType GetPrimaryAssetType();

};

