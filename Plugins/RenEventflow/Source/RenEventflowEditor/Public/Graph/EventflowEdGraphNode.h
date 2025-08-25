// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"

// Project Headers

// Generated Headers
#include "EventflowEdGraphNode.generated.h"

// Forward Declarations
class UEventflowNodeData;


#define EVENTFLOW_NODE_TYPE(NodeClass, NodeTypeName) \
public: \
	static FName StaticNodeType() \
	{ \
		return FName(TEXT(NodeTypeName)); \
	} \
protected: \
	virtual FName GetStaticNodeTypeInternal() const override \
	{ \
		return StaticNodeType(); \
	}


UCLASS()
class UEventflowEdGraphNode : public UEdGraphNode
{

	GENERATED_BODY()

public:

	virtual FName GetNodeType() const
	{
		return GetStaticNodeTypeInternal();
	}

	void SetAssetNodeData(UEventflowNodeData* AssetNodeData);
	UEventflowNodeData* GetAssetNodeData() const;

	virtual void CreateDefaultPins();

	void SyncPins();

public:

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool CanUserDeleteNode() const override;
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;

protected:

	virtual bool CanCreateRuntimeInputPins() const;
	virtual bool CanCreateRuntimeOutputPins() const;

	virtual FName GetStaticNodeTypeInternal() const { return NAME_None; };

private:

	UPROPERTY()
	UEventflowNodeData* CachedAssetNodeData = nullptr;

};



UCLASS()
class UEventflowEdGraphBeginNode : public UEventflowEdGraphNode
{

	GENERATED_BODY()

	EVENTFLOW_NODE_TYPE(UEventflowEdGraphBeginNode, "REN.EF.DEFAULT.BEGIN")

public:


	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override
	{
		return FText::FromString(TEXT("Begin"));
	}

	virtual FLinearColor GetNodeTitleColor() const override
	{
		return FLinearColor(0.0f, 1.0f, 0.0f);
	}

	virtual void CreateDefaultPins() override;

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

	EVENTFLOW_NODE_TYPE(UEventflowEdGraphEndNode, "REN.EF.DEFAULT.END")

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




