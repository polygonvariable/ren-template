// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "EdGraph/EdGraphNode.h"
#include "InstancedStruct.h"

// Generated Headers
#include "EventflowEdGraphNode.generated.h"

// Module Macros
#define REN_API RENEVENTFLOWED_API

// Forward Declarations
class UEventflowTask;


/*
 *
 */
UCLASS()
class REN_API UEventflowEdGraphNode : public UEdGraphNode
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UEventflowTask> PrimaryTask = nullptr;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UEventflowTask>> SecondaryTasks;


	virtual bool GetIsEntryNode() const;
	virtual FText GetNodeDescription() const;

	virtual TArray<FText> GetRuntimeInputPins() const;
	virtual TArray<FText> GetRuntimeOutputPins() const;

	virtual void SyncRuntimePins();

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

