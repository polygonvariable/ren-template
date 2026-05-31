// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "DialogueNodeData.h"
#include "Graph/EventflowEdGraphNode.h"

// Generated Headers
#include "DialogueEdGraphNode.generated.h"

// Forward Declarations
class UEventflowNodeData;



UCLASS()
class UDialogueEdNode_Base : public UEventflowEdGraphNode
{

	GENERATED_BODY()

};


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

