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
class RENEVENTFLOWEDITOR_API UEventflowEdGraphNode : public UEdGraphNode
{

	GENERATED_BODY()

public:

	virtual FName GetNodeType() const;
	virtual FText GetNodeDescription() const;
	virtual bool IsEntryNode() const;

	void SetAssetNodeData(UEventflowNodeData* AssetNodeData);
	UEventflowNodeData* GetAssetNodeData() const;

	void SyncPins();

protected:

	virtual bool CanCreateRuntimeInputPins() const;
	virtual bool CanCreateRuntimeOutputPins() const;

	virtual FName GetStaticNodeTypeInternal() const;
	virtual FText GetStaticNodeTitleInternal() const;
	virtual FText GetStaticNodeDescriptionInternal() const;

	void CreatePinHelper(FText FriendlyName, EEdGraphPinDirection Direction, const TArray<FText>* Options, const TArray<UEdGraphPin*>& CachedLinks);

	FString LimitTextLength(const FString& InText, int MaxCharacters) const;

public:

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool CanUserDeleteNode() const override;
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;

private:

	UPROPERTY()
	UEventflowNodeData* CachedAssetNodeData = nullptr;

};

