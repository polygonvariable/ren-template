// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"
#include "RenEventflow/Public/EventflowNodeData.h"
#include "RenEventflow/Public/EventflowNodeTask.h"

// Generated Headers
#include "QuestAsset.generated.h"

// Forward Declarations
class UQuestSubsystem;



UCLASS(Abstract, MinimalAPI)
class UQuestNodeTask : public UEventflowNodeTask
{

	GENERATED_BODY()

protected:

	virtual UQuestSubsystem* GetQuestSubsystem() const;

};


UCLASS(Abstract, MinimalAPI)
class UQuestNodeTask_StartQuest : public UQuestNodeTask
{

	GENERATED_BODY()

protected:

	virtual void OnActionStarted_Implementation() override;

};

UCLASS(Abstract, MinimalAPI)
class UQuestNodeTask_Objective : public UQuestNodeTask
{

	GENERATED_BODY()

protected:

	virtual void OnActionStarted_Implementation() override;

};













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

	UPROPERTY(EditAnywhere)
	TSubclassOf<UQuestNodeTask> QuestTask;

public:

	virtual const TArray<FText>* GetOutputOptions() const override;

};




UCLASS(BlueprintType)
class RENQUEST_API UQuestGraph : public UEventflowAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGuid QuestId = FGuid::NewGuid();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText QuestTitle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText QuestDescription;

};


UCLASS(Blueprintable, BlueprintType)
class RENQUEST_API UQuestGraphBlueprint : public UObject
{

	GENERATED_BODY()

public:

	void SetOwnedGraph(UQuestGraph* AssetGraph);

};

