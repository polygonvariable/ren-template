// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Asset/EventflowAsset.h"

// Generated Headers
#include "DialogueAsset.generated.h"

// Forward Declarations
class UUserWidget;


/**
 * 
 */
UCLASS(MinimalAPI, BlueprintType)
class UDialogueAsset : public UEventflowAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FText Summary;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> DialogueWidgetClass;

};

