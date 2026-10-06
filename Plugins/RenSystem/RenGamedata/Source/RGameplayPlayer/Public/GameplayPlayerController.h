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

	UPROPERTY(EditAnywhere, meta = (Categories = "InputMode"))
	FGameplayTag InputModeGameTag = FGameplayTag::RequestGameplayTag(TEXT("InputMode.Game"));

	UPROPERTY(EditAnywhere, meta = (Categories = "InputMode"))
	FGameplayTag InputModeGameUITag = FGameplayTag::RequestGameplayTag(TEXT("InputMode.GameUI"));

	UPROPERTY(EditAnywhere, meta = (Categories = "InputMode"))
	FGameplayTag InputModeUITag = FGameplayTag::RequestGameplayTag(TEXT("InputMode.UI"));

	IGameplayModeProvider* GameplayMode;


	// ~ Binding
	void HandleOnInputModeTagChanged(bool bAdded);
	// ~ End of Binding

};

