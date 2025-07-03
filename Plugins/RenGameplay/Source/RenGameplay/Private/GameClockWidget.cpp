// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "GameClockWidget.h"

// Engine Headers
#include "Components/TextBlock.h"

// Project Headers
#include "GameClockSubsystem.h"
#include "RenCore/Public/Library/MiscLibrary.h"



void UGameClockWidget::HandleClockTick(float Time)
{
	TimeTextBlock->SetText(FText::FromString(GameClockSubsystem->GetFormattedTime(Format)));
}

void UGameClockWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameClockSubsystem = GetSubsystem<UGameClockSubsystem>(GetWorld());
	if (IsValid(TimeTextBlock) && IsValid(GameClockSubsystem))
	{
		GameClockSubsystem->OnGameTimeChanged.AddDynamic(this, &UGameClockWidget::HandleClockTick);
	}
}

void UGameClockWidget::NativeDestruct()
{
	if (IsValid(GameClockSubsystem))
	{
		GameClockSubsystem->OnGameTimeChanged.RemoveAll(this);
	}

	Super::NativeDestruct();
}

