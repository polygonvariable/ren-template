// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers
#include "EventflowNode.generated.h"

// Forward Declarations
class UEventflowNodeData;
class UEventflowPin;



/**
 *
 *
 */
UCLASS(BlueprintType)
class RENEVENTFLOW_API UEventflowNode : public UObject
{

	GENERATED_BODY()

public:

	UPROPERTY()
	FGuid NodeId;

	UPROPERTY()
	TArray<TObjectPtr<UEventflowPin>> NodeInputs;

	UPROPERTY()
	TArray<TObjectPtr<UEventflowPin>> NodeOutputs;

#if WITH_EDITORONLY_DATA

	UPROPERTY()
	FName NodeType;

	UPROPERTY()
	FVector2D NodePosition;

#endif

	UPROPERTY()
	TObjectPtr<UEventflowNodeData> NodeData = nullptr;

	UEventflowNode* GetNextNodeAt(int Index) const;

};

