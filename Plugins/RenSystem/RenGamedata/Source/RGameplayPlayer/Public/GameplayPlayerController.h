// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"

// Generated Headers
#include "GameplayPlayerController.generated.h"

// Forward Declaration
class IGameplayModeProvider;


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

protected:

	UPROPERTY(EditAnywhere, meta = (Categories = "Gameplay"))
	FGameplayTag MouseVisiblityTag = FGameplayTag::RequestGameplayTag(TEXT("Gameplay.Mouse.Visible"));

	IGameplayModeProvider* GameplayMode;


	// ~ Binding
	void HandleOnMouseVisiblityTagChanged(bool bAdded);
	// ~ End of Binding

};

