// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Generated Headers
#include "DialogueSettings.generated.h"


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, meta = (DisplayName = "RSystem - Dialogue"))
class UDialogueSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UDialogueSettings(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(Config, EditDefaultsOnly, Category = "Gameplay Mode")
	FName DialogueMode;

public:

	static const UDialogueSettings* Get();

};

