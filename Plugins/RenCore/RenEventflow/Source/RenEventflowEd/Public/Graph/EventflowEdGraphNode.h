// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "EdGraph/EdGraphNode.h"

// Project Headers
#include "Definition/EventflowTransition.h"

// Generated Headers
#include "EventflowEdGraphNode.generated.h"

// Module Macros
#define REN_API RENEVENTFLOWED_API

// Forward Declarations
class UEventflowPrimaryTask;
class UEventflowSubTask;


/*
 *
 */
UCLASS()
class REN_API UEventflowEdGraphNode : public UEdGraphNode
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Task Transition")
	TArray<FEventflowTransition> TaskTransitions;

	UPROPERTY(EditAnywhere, Category = "Sub Task Condition")
	TMap<EFSMResult, FEventflowCondition_TaskState> SubTaskConditions;


	virtual UEventflowPrimaryTask* GetTask() const;
	virtual void SetTask(UEventflowPrimaryTask* Task);

	virtual TArray<UEventflowSubTask*> GetSubTasks() const;
	virtual void SetSubTasks(const TArray<UEventflowSubTask*>& Tasks);

	virtual bool IsEntryNode() const;
	virtual FText GetNodeDescription() const;

	virtual TArray<FText> GetRuntimeInputPins() const;
	virtual TArray<FText> GetRuntimeOutputPins() const;
	virtual void SyncRuntimeData();

	// ~ UEdGraphNode
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool CanUserDeleteNode() const override;
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	// ~ End of UEdGraphNode

protected:

	void CreateRuntimePins(const TArray<FText>& PinNames, EEdGraphPinDirection Direction);
	void FuzzyMatchRuntimePins(const TArray<TPair<FString, TArray<UEdGraphPin*>>> FuzzyPins);

};


// Module Macros
#undef REN_API

