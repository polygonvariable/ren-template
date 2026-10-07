// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Graph/EventflowEdGraphNode.h"

// Generated Headers
#include "DialogueEdGraphNode.generated.h"

// Forward Declarations
class UEventflowNodeData;
class UDialogueTask_Default;
class UDialogueTask_Branch;
class UDialogueTask_Begin;
class UDialogueTask_End;


/*
 *
 */
UCLASS()
class UDialogueEdNode_Base : public UEventflowEdGraphNode
{

	GENERATED_BODY()

public:

	UDialogueEdNode_Base();

};


/*
 *
 */
UCLASS()
class UDialogueEdNode_Begin : public UDialogueEdNode_Base
{

	GENERATED_BODY()

public:

	UDialogueEdNode_Begin();

	// ~ UEventflowEdGraphNode
	virtual TSubclassOf<UEventflowNodeTask> GetTaskClass() const;
	// ~ UEventflowEdGraphNode
	
	// ~ UEdGraphNode
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AllocateDefaultPins() override;
	// ~ End of UEdGraphNode

};


/*
 *
 */
UCLASS()
class UDialogueEdNode_End : public UDialogueEdNode_Base
{

	GENERATED_BODY()

public:

	UDialogueEdNode_End();

	// ~ UEventflowEdGraphNode
	virtual TSubclassOf<UEventflowNodeTask> GetTaskClass() const;
	// ~ UEventflowEdGraphNode
	
	// ~ UEdGraphNode
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AllocateDefaultPins() override;
	// ~ End of UEdGraphNode

};


/*
 *
 */
UCLASS()
class UDialogueEdNode_Dialogue : public UDialogueEdNode_Base
{

	GENERATED_BODY()

public:

	UDialogueEdNode_Dialogue();

	// ~ UEventflowEdGraphNode
	virtual TSubclassOf<UEventflowNodeTask> GetTaskClass() const;
	// ~ UEventflowEdGraphNode
	
	// ~ UEdGraphNode
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AllocateDefaultPins() override;
	// ~ End of UEdGraphNode

};


/*
 *
 */
UCLASS()
class UDialogueEdNode_Branch : public UEventflowEdGraphNode
{

	GENERATED_BODY()

public:

	UDialogueEdNode_Branch();

	// ~ UEventflowEdGraphNode
	virtual TSubclassOf<UEventflowNodeTask> GetTaskClass() const;
	virtual TArray<FText> GetRuntimeOutputPins() const override;
	// ~ End of UEventflowEdGraphNode

	// ~ UEdGraphNode
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AllocateDefaultPins() override;
	// ~ End of UEdGraphNode

};

