// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "DialogueData.generated.h"


/**
 *
 */
USTRUCT()
struct FDialogueSpeaker
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere, meta = (AssetBundles = "Dialogue"))
	TSoftObjectPtr<UTexture2D> Image;

};


/**
 * 
 */
USTRUCT()
struct FDialogueData
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta = (MultiLine = true))
	FText Content;

	UPROPERTY(EditAnywhere, meta = (AssetBundles = "Dialogue"))
	TSoftObjectPtr<USoundBase> Audio;

};

