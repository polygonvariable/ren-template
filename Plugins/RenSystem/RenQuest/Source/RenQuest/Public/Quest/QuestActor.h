// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

// Project Headers
#include "RenQuest/Public/Quest/QuestDelegate.h"
#include "RenQuest/Public/Quest/QuestInterface.h"

// Generated Headers
#include "QuestActor.generated.h"

// Forward Declarations
class UEventflowNode;



/**
 *
 * 
 */
UCLASS(Abstract)
class AQuestActor : public AActor, public IQuestObjectiveInterface_BP
{

	GENERATED_BODY()

public:

	// ~ IQuestObjectiveInterface
	virtual void InitializeObjective() override;
	virtual void CompleteObjective() override;
	virtual void DestroyObjective() override;

	virtual void SetObjectiveNode(UEventflowNode* Node) override;
	virtual UEventflowNode* GetObjectiveNode() override;

	virtual FOnQuestObjectiveCompleted& GetOnQuestObjectiveCompleted() override;
	// ~ End of IQuestObjectiveInterface

protected:

	FOnQuestObjectiveCompleted OnQuestObjectiveCompleted;
	TWeakObjectPtr<UEventflowNode> ObjectiveNode;

	// ~ IQuestObjectiveInterface_BP
	virtual void BP_InitializeObjective_Implementation() override;
	virtual void BP_DestroyObjective_Implementation() override;

	UFUNCTION(BlueprintCallable)
	virtual void BP_CompleteObjective() override;
	// ~ End of IQuestObjectiveInterface_BP

};

