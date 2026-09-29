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
	virtual void SetTask(UEventflowPrimaryTask* InTask) override;
	// ~ End of UEventflowEdGraphNode

protected:

	UPROPERTY(EditAnywhere, Instanced, Category = "Task")
	TObjectPtr<UDialogueTask_Default> Task;

};


/*
 *
 */
UCLASS()
class UDialogueEdNode_Begin : public UDialogueEdNode_Base
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
class UDialogueEdNode_End : public UDialogueEdNode_Base
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
class UDialogueEdNode_Dialogue : public UDialogueEdNode_Base
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
class UDialogueEdNode_Branch : public UEventflowEdGraphNode
{

	GENERATED_BODY()

public:

	// ~ UEventflowEdGraphNode
	virtual UEventflowPrimaryTask* GetTask() const override;
	virtual void SetTask(UEventflowPrimaryTask* InTask) override;
	virtual TArray<FText> GetRuntimeOutputPins() const override;
	// ~ End of UEventflowEdGraphNode

	// ~ UEdGraphNode
	virtual FText GetNodeDescription() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AllocateDefaultPins() override;
	// ~ End of UEdGraphNode

protected:

	UPROPERTY(EditAnywhere, Instanced, Category = "Task")
	TObjectPtr<UDialogueTask_Branch> Task;

};

