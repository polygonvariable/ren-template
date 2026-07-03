// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Graph/EventflowEdGraphNode.h"

// Generated Headers
#include "QuestEdGraphNode.generated.h"

// Forward Declarations
class UEventflowNodeData;
class UQuestTask_TriggerZone;
class UQuestPrimaryTask;
class UQuestSubTask;
class UQuestEngine;




/*
 *
 */
UCLASS()
class UQuestEdNode_Base : public UEventflowEdGraphNode
{

	GENERATED_BODY()

};






/*
 *
 */
UCLASS()
class UQuestEdNode_Begin : public UQuestEdNode_Base
{

	GENERATED_BODY()

public:

	// ~ UEventflowEdGraphNode
	virtual UEventflowPrimaryTask* GetTask() const override;
	virtual void SetTask(UEventflowPrimaryTask* Task) override;
	// ~ End of UEventflowEdGraphNode

	// ~ UEdGraphNode
	virtual FText GetNodeDescription() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void AllocateDefaultPins() override;
	// ~ End of UEdGraphNode

protected:

	//UPROPERTY(EditAnywhere, Instanced)
	//TObjectPtr<UQuestTask_Begin> PrimaryTask = nullptr;

};

/*
 *
 */
UCLASS()
class UQuestEdNode_End : public UQuestEdNode_Base
{

	GENERATED_BODY()

public:

	// ~ UEventflowEdGraphNode
	virtual UEventflowPrimaryTask* GetTask() const override;
	virtual void SetTask(UEventflowPrimaryTask* Task) override;
	// ~ End of UEventflowEdGraphNode

	// ~ UEdGraphNode
	virtual FText GetNodeDescription() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void AllocateDefaultPins() override;
	// ~ End of UEdGraphNode

protected:

	//UPROPERTY(EditAnywhere, Instanced)
	//TObjectPtr<UQuestTask_End> PrimaryTask = nullptr;

};




/*
 *
 */
UCLASS()
class UQuestEdNode_GoToPlace : public UQuestEdNode_Base
{

	GENERATED_BODY()

public:

	// ~ UEventflowEdGraphNode
	virtual TArray<FText> GetRuntimeOutputPins() const override;

	virtual UEventflowPrimaryTask* GetTask() const override;
	virtual void SetTask(UEventflowPrimaryTask* InTask) override;

	virtual TArray<UEventflowSubTask*> GetSubTasks() const override;
	virtual void SetSubTasks(const TArray<UEventflowSubTask*>& Tasks) override;

	virtual bool IsEntryNode() const override;
	// ~ End of UEventflowEdGraphNode

	// ~ UEdGraphNode
	virtual FText GetNodeDescription() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void AllocateDefaultPins() override;
	// ~ End of UEdGraphNode

protected:

	UPROPERTY(EditAnywhere, Instanced, Category = "Task")
	TObjectPtr<UQuestTask_TriggerZone> Task = nullptr;

	UPROPERTY(EditAnywhere, Instanced, Category = "Sub Tasks")
	TArray<TObjectPtr<UQuestSubTask>> SubTasks;

};




/*
 *
 */
UCLASS()
class UQuestEdNode_SubGraph : public UQuestEdNode_Base
{

	GENERATED_BODY()

public:

	// ~ UEventflowEdGraphNode
	virtual UEventflowPrimaryTask* GetTask() const override;
	virtual void SetTask(UEventflowPrimaryTask* Task) override;
	// ~ End of UEventflowEdGraphNode

	// ~ UEdGraphNode
	virtual FText GetNodeDescription() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void AllocateDefaultPins() override;
	// ~ End of UEdGraphNode

protected:

	//UPROPERTY(EditAnywhere, Instanced)
	//TObjectPtr<UQuestObjective_SubGraph> PrimaryTask = nullptr;

};
