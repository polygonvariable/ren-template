// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEventflow/Public/EventflowNodeData.h"

// Generated Headers
#include "DialogueNodeData.generated.h"

// Forward Declarations



/**
 * 
 */
UCLASS(BlueprintType)
class RENDIALOGUE_API UDialogueNodeData : public UEventflowNodeData
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FText DialogueTitle;

	UPROPERTY(EditAnywhere)
	FText DialogueContent;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> DialogueAvatar;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USoundBase> DialogueSound;

	UPROPERTY(EditAnywhere)
	TArray<FText> OutputOptions;

public:

	virtual const TArray<FText>* GetOutputOptions() const override
	{
		return &OutputOptions;
	}

};

