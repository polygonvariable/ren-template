// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"

// Project Headers
#include "RenEventflowEditor/Public/EventflowEdMacro.h"

// Generated Headers
#include "EventflowEdGraphNode.generated.h"

// Forward Declarations
class UEventflowNodeData;



UCLASS()
class RENEVENTFLOWEDITOR_API UEventflowEdGraphNode : public UEdGraphNode
{

	GENERATED_BODY()

public:

	virtual FName GetNodeType() const;
	virtual FText GetNodeDescription() const;

	void SetAssetNodeData(UEventflowNodeData* AssetNodeData);
	UEventflowNodeData* GetAssetNodeData() const;

	void SyncPins();

protected:

	virtual bool CanCreateRuntimeInputPins() const;
	virtual bool CanCreateRuntimeOutputPins() const;

	virtual FName GetStaticNodeTypeInternal() const;
	virtual FText GetStaticNodeTitleInternal() const;
	virtual FText GetStaticNodeDescriptionInternal() const;

public:

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool CanUserDeleteNode() const override;
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;

private:

	UPROPERTY()
	UEventflowNodeData* CachedAssetNodeData = nullptr;

};


/*
UCLASS()
class UEventflowEdGraphBeginNode : public UEventflowEdGraphNode
{

	GENERATED_BODY()

	EVENTFLOW_NODE_DECLARE_TYPE("REN.EF.DEFAULT.BEGIN")
	EVENTFLOW_NODE_DECLARE_TITLE("REN.EF.DEFAULT.BEGIN")
	EVENTFLOW_NODE_DECLARE_DESCRIPTION("REN.EF.DEFAULT.BEGIN")

public:

	virtual FLinearColor GetNodeTitleColor() const override
	{
		return FLinearColor(0.0f, 1.0f, 0.0f);
	}

protected:

	virtual bool CanCreateRuntimeOutputPins() const override
	{
		return true;
	}

};

UCLASS()
class UEventflowEdGraphEndNode : public UEventflowEdGraphNode
{

	GENERATED_BODY()

	EVENTFLOW_NODE_DECLARE(UEventflowEdGraphEndNode, "REN.EF.DEFAULT.END")

public:


	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override
	{
		return FText::FromString(TEXT("End"));
	}

	virtual FLinearColor GetNodeTitleColor() const override
	{
		return FLinearColor(0.0f, 0.0f, 1.0f);
	}

	virtual void CreateDefaultPins() override;

protected:

	virtual bool CanCreateRuntimeInputPins() const  override
	{
		return true;
	}

};
*/

