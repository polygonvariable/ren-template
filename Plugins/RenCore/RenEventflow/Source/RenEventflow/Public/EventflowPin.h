// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers
#include "EventflowPin.generated.h"

// Forward Declarations
class UEventflowNode;



UCLASS()
class RENEVENTFLOW_API UEventflowPin : public UObject
{

	GENERATED_BODY()

public:

	UPROPERTY()
	FName PinName;

	UPROPERTY()
	FName PinCategory;

	UPROPERTY()
	FText PinFriendlyName;

	UPROPERTY()
	FGuid PinGuid;

	UPROPERTY()
	bool bPinIsConst = false;

	// UPROPERTY()
	// UEventflowNode* PinParentNode = nullptr;

	UPROPERTY()
	TObjectPtr<UEventflowPin> PinLinkedTo = nullptr;

};

