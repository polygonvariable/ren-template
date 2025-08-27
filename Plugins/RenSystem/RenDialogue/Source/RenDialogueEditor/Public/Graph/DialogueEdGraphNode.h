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

	EVENTFLOW_NODE_DECLARE_TYPE("REN.EF.DIA.BEGIN")
	EVENTFLOW_NODE_DECLARE_TITLE("Begin")
	EVENTFLOW_NODE_DECLARE_DESCRIPTION("Starts a conversation")

public:

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

	EVENTFLOW_NODE_DECLARE_TYPE("REN.EF.DIA.END")
	EVENTFLOW_NODE_DECLARE_TITLE("End")
	EVENTFLOW_NODE_DECLARE_DESCRIPTION("Ends a conversation")

public:

	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AllocateDefaultPins() override;

protected:

	virtual bool CanCreateRuntimeOutputPins() const override;

};



UCLASS()
class UDialogueEdDialogNode : public UEventflowEdGraphNode
{

	GENERATED_BODY()

	EVENTFLOW_NODE_DECLARE_TYPE("REN.EF.DIA.DIALOG")
	EVENTFLOW_NODE_DECLARE_TITLE("Dialog")
	EVENTFLOW_NODE_DECLARE_DESCRIPTION("Conversation dialog")

public:

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AllocateDefaultPins() override;

protected:

	virtual bool CanCreateRuntimeOutputPins() const override;

};

