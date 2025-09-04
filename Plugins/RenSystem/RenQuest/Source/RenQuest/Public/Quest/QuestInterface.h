// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

#include "UObject/Interface.h"

// Project Headers
#include "RenQuest/Public/Quest/QuestDelegate.h"

// Generated Headers
#include "QuestInterface.generated.h"

// Forward Declarations
class UEventflowNode;



UINTERFACE(MinimalAPI)
class UQuestObjectiveInterface : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class IQuestObjectiveInterface
{

	GENERATED_BODY()

public:

	virtual FOnQuestObjectiveCompleted& GetOnQuestObjectiveCompleted() = 0;

	virtual void InitializeObjective() = 0;
	virtual void CompleteObjective() = 0;
	virtual void DestroyObjective() = 0;

	virtual void SetObjectiveNode(UEventflowNode* Node) = 0;
	virtual UEventflowNode* GetObjectiveNode() = 0;

};



UINTERFACE(BlueprintType, MinimalAPI)
class UQuestObjectiveInterface_BP : public UQuestObjectiveInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class IQuestObjectiveInterface_BP : public IQuestObjectiveInterface
{

	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintNativeEvent)
	void BP_InitializeObjective();

	UFUNCTION(BlueprintNativeEvent)
	void BP_DestroyObjective();

	virtual void BP_CompleteObjective() = 0;

};

