// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "GameplayPlayerController.h"

// Project Headers
#include "GameplayModeProvider.h"
#include "SubsystemLibrary.h"


void AGameplayPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GameplayMode = FSubsystemLibrary::GetSubsystemInterface<IGameplayModeProvider>(GetWorld());
	if (!GameplayMode)
	{
		return;
	}

	HandleOnInputModeTagChanged(true);

	GameplayMode->RegisterTagNotify(InputModeGameTag, FOnGameplayModeTagChanged::FDelegate::CreateUObject(this, &AGameplayPlayerController::HandleOnInputModeTagChanged));
	GameplayMode->RegisterTagNotify(InputModeGameUITag, FOnGameplayModeTagChanged::FDelegate::CreateUObject(this, &AGameplayPlayerController::HandleOnInputModeTagChanged));
	GameplayMode->RegisterTagNotify(InputModeUITag, FOnGameplayModeTagChanged::FDelegate::CreateUObject(this, &AGameplayPlayerController::HandleOnInputModeTagChanged));
}

void AGameplayPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GameplayMode)
	{
		GameplayMode->UnregisterTagNotify(InputModeGameTag, this);
		GameplayMode->UnregisterTagNotify(InputModeGameUITag, this);
		GameplayMode->UnregisterTagNotify(InputModeUITag, this);
	}
	GameplayMode = nullptr;

	Super::EndPlay(EndPlayReason);
}

void AGameplayPlayerController::HandleOnInputModeTagChanged(bool bAdded)
{
	const FGameplayTag& InputTag = GameplayMode->GetInputModeTag();

	if (InputTag.MatchesTagExact(InputModeGameTag))
	{
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
	else if (InputTag.MatchesTagExact(InputModeGameUITag))
	{
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());
	}
	else if (InputTag.MatchesTagExact(InputModeUITag))
	{
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

