// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "GameClockWidget.h"

// Engine Headers
#include "Components/TextBlock.h"

// Project Headers
#include "GameClockSubsystem.h"



void UGameClockWidget::HandleTimeChanged(float Time)
{
	TimeTextBlock->SetText(FText::FromString(GameClockSubsystem->GetFormattedTime(Format)));
}

void UGameClockWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		UGameClockSubsystem* ClockSubsystem = World->GetSubsystem<UGameClockSubsystem>();
		if (IsValid(ClockSubsystem) && IsValid(TimeTextBlock))
		{
			GameClockSubsystem = ClockSubsystem;
			GameClockSubsystem->OnGameTimeChanged.AddDynamic(this, &UGameClockWidget::HandleTimeChanged);

			HandleTimeChanged(0.0f);
		}
	}
}

void UGameClockWidget::NativeDestruct()
{
	if (IsValid(GameClockSubsystem))
	{
		GameClockSubsystem->OnGameTimeChanged.RemoveAll(this);
	}
	GameClockSubsystem = nullptr;

	Super::NativeDestruct();
}

