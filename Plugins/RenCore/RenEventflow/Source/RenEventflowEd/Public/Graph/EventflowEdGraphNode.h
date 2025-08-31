// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"

// Project Headers

// Generated Headers
#include "EventflowEdGraphNode.generated.h"

// Forward Declarations
class UEventflowBlueprint;

class UEventflowNodeData;



UCLASS()
class RENEVENTFLOWED_API UEventflowEdGraphNode : public UEdGraphNode
{

	GENERATED_BODY()

public:

	virtual FName GetNodeType() const;
	virtual FText GetNodeDescription() const;
	virtual TSubclassOf<UEventflowNodeData> GetNodeDataClass() const;

	virtual bool IsEntryNode() const;

	void SetNodeData(UEventflowNodeData* NodeData);
	UEventflowNodeData* GetNodeData() const;

	void SyncPins();
	void SyncBlueprintGraph(TSubclassOf<UEventflowBlueprint> BlueprintClass);

protected:

	virtual bool CanCreateRuntimeInputPins() const;
	virtual bool CanCreateRuntimeOutputPins() const;

	void CreatePinHelper(FText FriendlyName, EEdGraphPinDirection Direction, const TArray<FText>* Options, const TArray<UEdGraphPin*>& CachedLinks);

	FString LimitTextLength(const FString& InText, int MaxCharacters) const;

public:

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool CanUserDeleteNode() const override;
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;

private:

	UPROPERTY()
	UEventflowNodeData* CachedNodeData = nullptr;

};

