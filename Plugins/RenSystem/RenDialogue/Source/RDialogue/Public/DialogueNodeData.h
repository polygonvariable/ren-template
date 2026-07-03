// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "DialogueNodeData.generated.h"


/**
 * 
 */
USTRUCT()
struct FDialogueData
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FText Speaker;

	UPROPERTY(EditAnywhere, meta = (AssetBundles = "Dialogue"))
	TSoftObjectPtr<UTexture2D> SpeakerImage;

	UPROPERTY(EditAnywhere)
	FText Content;

	UPROPERTY(EditAnywhere, meta = (AssetBundles = "Dialogue"))
	TSoftObjectPtr<USoundBase> Audio;

	UPROPERTY(EditAnywhere)
	TArray<FText> Options;

};

