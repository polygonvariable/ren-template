// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "GameplayContextAction.h"
#include "System/Flow/Task/QuestPrimaryTask.h"

// Generated Headers
#include "QuestTask_GameplayContext.generated.h"


/**
 *
 */
UCLASS(MinimalAPI)
class UQuestTask_GameplayContext : public UQuestPrimaryTask
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Gameplay Context", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FGameplayContextAction>> Contexts;

	// ~ UEventflowTask
	virtual void OnActive(EFSMState PreviousState) override;
	// ~ End of UEventflowTask

};


/**
 *
 */
UCLASS(MinimalAPI)
class UQuestTask_AddAssetInstance : public UQuestPrimaryTask
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	FName SourceId;

	UPROPERTY(EditAnywhere)
	FPrimaryAssetType AssetType;

	UPROPERTY(EditAnywhere)
	TMap<FPrimaryAssetId, int> Items;

	// ~ UEventflowTask
	virtual void OnActive(EFSMState PreviousState) override;
	// ~ End of UEventflowTask

};


/**
 *
 */
UCLASS(MinimalAPI)
class UQuestTask_ContainAssetInstance : public UQuestPrimaryTask
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	FName SourceId;

	UPROPERTY(EditAnywhere)
	FPrimaryAssetType AssetType;

	UPROPERTY(EditAnywhere)
	TMap<FPrimaryAssetId, int> Items;

	UPROPERTY(EditAnywhere)
	bool bContainsAll = false;

	// ~ UEventflowTask
	virtual void OnActive(EFSMState PreviousState) override;
	// ~ End of UEventflowTask

};

