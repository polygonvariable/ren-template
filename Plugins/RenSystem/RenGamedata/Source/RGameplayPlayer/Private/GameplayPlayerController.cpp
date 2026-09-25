// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "GameplayPlayerController.h"

// Project Headers
#include "Core/GameplayModeProvider.h"
#include "Util/SubsystemUtil.h"


void AGameplayPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GameplayMode = FSubsystemLibrary::GetSubsystemInterface<IGameplayModeProvider>(GetWorld());
	if (!GameplayMode)
	{
		return;
	}
	HandleOnMouseVisiblityTagChanged(GameplayMode->HasTagExact(MouseVisiblityTag));
	GameplayMode->RegisterTagNotify(MouseVisiblityTag, FOnGameplayModeTagChanged::FDelegate::CreateUObject(this, &AGameplayPlayerController::HandleOnMouseVisiblityTagChanged));
}

void AGameplayPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GameplayMode)
	{
		GameplayMode->UnregisterTagNotify(MouseVisiblityTag, this);
	}
	GameplayMode = nullptr;

	Super::EndPlay(EndPlayReason);
}

void AGameplayPlayerController::HandleOnMouseVisiblityTagChanged(bool bAdded)
{
	bShowMouseCursor = bAdded;
	if (bShowMouseCursor)
	{
		SetInputMode(FInputModeUIOnly());
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
	}
}

