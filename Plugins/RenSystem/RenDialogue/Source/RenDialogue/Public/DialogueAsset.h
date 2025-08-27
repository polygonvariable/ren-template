// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"
#include "RenEventflow/Public/EventflowNodeData.h"

// Generated Headers
#include "DialogueAsset.generated.h"

// Forward Declarations



UCLASS(BlueprintType)
class RENDIALOGUE_API UDialogueNodeData : public UEventflowNodeData
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FText Title;

	UPROPERTY(EditAnywhere)
	FText Content;

	UPROPERTY(EditAnywhere)
	TArray<FText> InputOptions;

	UPROPERTY(EditAnywhere)
	TArray<FText> OutputOptions;

	virtual const TArray<FText>* GetInputOptions() const override { return &InputOptions; };
	virtual const TArray<FText>* GetOutputOptions() const override { return &OutputOptions; };

};

UCLASS(BlueprintType)
class RENDIALOGUE_API UDialogueAsset : public UEventflowAsset
{

	GENERATED_BODY()

};

