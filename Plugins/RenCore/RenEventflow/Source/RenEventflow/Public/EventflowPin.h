// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers
#include "EventflowPin.generated.h"

// Forward Declarations
class UEventflowNode;



/**
 *
 *
 */
UCLASS()
class RENEVENTFLOW_API UEventflowPin : public UObject
{

	GENERATED_BODY()

public:

#if WITH_EDITORONLY_DATA

	UPROPERTY(VisibleAnywhere)
	FName PinName;

	UPROPERTY(VisibleAnywhere)
	FName PinCategory;

	UPROPERTY(VisibleAnywhere)
	FText PinFriendlyName;

	UPROPERTY(VisibleAnywhere)
	FGuid PinGuid;

	UPROPERTY(VisibleAnywhere)
	bool bPinIsConst = false;

	// UPROPERTY()
	// UEventflowNode* PinParentNode = nullptr;

#endif

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UEventflowPin> PinLinkedTo = nullptr;

};

