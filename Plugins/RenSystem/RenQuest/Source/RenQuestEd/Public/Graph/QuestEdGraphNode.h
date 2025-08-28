// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEventflowEd/Public/Graph/EventflowEdGraphNode.h"

// Generated Headers
#include "QuestEdGraphNode.generated.h"

// Forward Declarations
class UEventflowNodeData;



UCLASS()
class UQuestEdBeginNode : public UEventflowEdGraphNode
{

	GENERATED_BODY()

public:

	virtual FName GetNodeType() const override;
	virtual FText GetNodeDescription() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual TSubclassOf<UEventflowNodeData> GetNodeDataClass() const override;

	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AllocateDefaultPins() override;
	virtual bool IsEntryNode() const override;

};



UCLASS()
class UQuestEdObjectiveNode : public UEventflowEdGraphNode
{

	GENERATED_BODY()

public:

	virtual FName GetNodeType() const override;
	virtual FText GetNodeDescription() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual TSubclassOf<UEventflowNodeData> GetNodeDataClass() const override;

	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AllocateDefaultPins() override;

protected:

	virtual bool CanCreateRuntimeOutputPins() const override;

};



UCLASS()
class UQuestEdEndNode : public UEventflowEdGraphNode
{

	GENERATED_BODY()

public:

	virtual FName GetNodeType() const override;
	virtual FText GetNodeDescription() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual TSubclassOf<UEventflowNodeData> GetNodeDataClass() const override;

	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AllocateDefaultPins() override;
	
};

