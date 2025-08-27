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



UCLASS()
class RENEVENTFLOW_API UEventflowNode : public UObject
{

	GENERATED_BODY()

public:

	UPROPERTY()
	FGuid NodeGuid;

	UPROPERTY()
	TArray<TObjectPtr<UEventflowPin>> InputPins;

	UPROPERTY()
	TArray<TObjectPtr<UEventflowPin>> OutputPins;

	UPROPERTY()
	FName NodeType;

	UPROPERTY()
	FVector2D NodePosition;

	UPROPERTY()
	TObjectPtr<UEventflowNodeData> NodeData = nullptr;


	UEventflowNode* GetNextNodeAt(int Index) const;

};

