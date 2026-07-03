// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Graph/EventflowEdGraphNode.h"

// Generated Headers
#include "DialogueEdGraphNode.generated.h"

// Forward Declarations
class UEventflowNodeData;
class UDialogueTask;


/*
 *
 */
UCLASS()
class UDialogueEdNode_Base : public UEventflowEdGraphNode
{

	GENERATED_BODY()

public:

	// ~ UEventflowEdGraphNode
	virtual UEventflowPrimaryTask* GetTask() const override;
	virtual void SetTask(UEventflowPrimaryTask* Task) override;
	// ~ End of UEventflowEdGraphNode

protected:

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UDialogueTask> PrimaryTask = nullptr;

};


/*
 *
 */
UCLASS()
class UDialogueEdBeginNode : public UDialogueEdNode_Base
{

	GENERATED_BODY()

public:

	// ~ UEdGraphNode
	virtual FText GetNodeDescription() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AllocateDefaultPins() override;
	// ~ End of UEdGraphNode

};


/*
 *
 */
UCLASS()
class UDialogueEdEndNode : public UDialogueEdNode_Base
{

	GENERATED_BODY()

public:

	// ~ UEdGraphNode
	virtual FText GetNodeDescription() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AllocateDefaultPins() override;
	// ~ End of UEdGraphNode

};


/*
 *
 */
UCLASS()
class UDialogueEdDialogNode : public UDialogueEdNode_Base
{

	GENERATED_BODY()

public:

	// ~ UEdGraphNode
	virtual FText GetNodeDescription() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AllocateDefaultPins() override;
	// ~ End of UEdGraphNode

};


/*
 *
 */
UCLASS()
class UDialogueEdBranchNode : public UDialogueEdNode_Base
{

	GENERATED_BODY()

public:

	// ~ UEventflowEdGraphNode
	virtual TArray<FText> GetRuntimeOutputPins() const override;
	// ~ End of UEventflowEdGraphNode

	// ~ UEdGraphNode
	virtual FText GetNodeDescription() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AllocateDefaultPins() override;
	// ~ End of UEdGraphNode

};

