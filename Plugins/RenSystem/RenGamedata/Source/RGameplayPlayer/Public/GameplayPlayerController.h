// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/PlayerController.h"

// Generated Headers
#include "GameplayPlayerController.generated.h"


/**
 * 
 */
UCLASS(MinimalAPI)
class AGameplayPlayerController : public APlayerController
{

	GENERATED_BODY()

public:

	// ~ APlayerController
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~ End of APlayerController

};

