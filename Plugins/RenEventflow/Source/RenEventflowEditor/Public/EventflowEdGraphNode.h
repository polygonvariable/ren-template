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



UCLASS()
class UEventflowEdGraphNode : public UEdGraphNode
{

	GENERATED_BODY()

public:

	FString NType = TEXT("BASE");

	//UEdGraphPin* CreateCustomPin(EEdGraphPinDirection Direction, FName Title, bool bIsConst);

	UEdGraphPin* CreatePinHelper(EEdGraphPinDirection Direction, FName Title, FName Catergory, bool bIsConst);

	void SetAssetNodeData(UEventflowNodeData* AssetNodeData);
	UEventflowNodeData* GetAssetNodeData() const;

	virtual void CreateDefaultPins();

	void SyncOutputPins();

public:

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool CanUserDeleteNode() const override;
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;

private:

	UPROPERTY()
	UEventflowNodeData* CachedAssetNodeData = nullptr;

};



UCLASS()
class UEventflowEdGraphBeginNode : public UEventflowEdGraphNode
{

	GENERATED_BODY()

public:

	UEventflowEdGraphBeginNode()
	{
		NType = TEXT("BEGIN");
	}

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override
	{
		return FText::FromString(TEXT("Begin"));
	}

	virtual FLinearColor GetNodeTitleColor() const override
	{
		return FLinearColor(0.0f, 1.0f, 0.0f);
	}


	virtual void CreateDefaultPins() override;

};

UCLASS()
class UEventflowEdGraphEndNode : public UEventflowEdGraphNode
{

	GENERATED_BODY()

public:

	UEventflowEdGraphEndNode()
	{
		NType = TEXT("END");
	}

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override
	{
		return FText::FromString(TEXT("End"));
	}

	virtual FLinearColor GetNodeTitleColor() const override
	{
		return FLinearColor(0.0f, 0.0f, 1.0f);
	}
	virtual void CreateDefaultPins() override;

};




