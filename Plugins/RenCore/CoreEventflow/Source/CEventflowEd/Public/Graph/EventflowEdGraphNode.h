// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "EdGraph/EdGraphNode.h"

// Project Headers
#include "LuauSourceCode.h"
#include "Type/EventflowTransition.h"

// Generated Headers
#include "EventflowEdGraphNode.generated.h"

// Forward Declarations
class UEventflowNodeTask;
class UEventflowSubTask;


/*
 *
 */
UCLASS()
class CEVENTFLOWED_API UEventflowEdGraphNode : public UEdGraphNode
{

	GENERATED_BODY()

public:

	virtual UEventflowNodeTask* GetTask() const;
	virtual void SetTask(UEventflowNodeTask* InTask);

	virtual TSubclassOf<UEventflowNodeTask> GetTaskClass() const;
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

	FText NodeTitle;

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UEventflowNodeTask> NodeTask = nullptr;


	UEventflowNodeTask* EnsureTask(UEventflowNodeTask* InTask, UClass* TaskClass);

	void CreateRuntimePins(const TArray<FText>& PinNames, EEdGraphPinDirection Direction);
	void FuzzyMatchRuntimePins(const TArray<TPair<FString, TArray<UEdGraphPin*>>> FuzzyPins);

};

