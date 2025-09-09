// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers
#include "EventflowData.generated.h"

// Forward Declarations
class UEventflowNode;



/**
 *
 *
 */
UCLASS()
class RENEVENTFLOW_API UEventflowData : public UObject
{

	GENERATED_BODY()

public:

	UPROPERTY()
	TMap<FGuid, TObjectPtr<UEventflowNode>> NodeList;

	UPROPERTY()
	FGuid NodeEntry = FGuid();

	const UEventflowNode* GetEntryNode() const;

};

