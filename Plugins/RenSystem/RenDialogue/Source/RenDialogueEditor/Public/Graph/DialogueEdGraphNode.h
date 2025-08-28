// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEventflowEditor/Public/EventflowEdMacro.h"
#include "RenEventflowEditor/Public/Graph/EventflowEdGraphNode.h"

// Generated Headers
#include "DialogueEdGraphNode.generated.h"

// Forward Declarations
class UEventflowNodeData;



UCLASS()
class UDialogueEdBeginNode : public UEventflowEdGraphNode
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

protected:

	virtual bool CanCreateRuntimeOutputPins() const override;

};



UCLASS()
class UDialogueEdEndNode : public UEventflowEdGraphNode
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
class UDialogueEdDialogNode : public UEventflowEdGraphNode
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

