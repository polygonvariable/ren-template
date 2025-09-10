// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "ClockWidget.h"

// Engine Headers
#include "Components/TextBlock.h"

// Project Headers
#include "ClockSubsystem.h"



void UClockWidget::HandleTimeChanged(float Time)
{
	TimeTextBlock->SetText(FText::FromString(ClockSubsystem->GetFormattedTime(Format)));
}

void UClockWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		UClockSubsystem* ClockSubsystemPtr = World->GetSubsystem<UClockSubsystem>();
		if (IsValid(ClockSubsystemPtr) && IsValid(TimeTextBlock))
		{
			ClockSubsystem = ClockSubsystemPtr;
			ClockSubsystem->GetClockDelegates().OnTimeChanged.AddUObject(this, &UClockWidget::HandleTimeChanged);

			HandleTimeChanged(0.0f);
		}
	}
}

void UClockWidget::NativeDestruct()
{
	if (IsValid(ClockSubsystem))
	{
		ClockSubsystem->GetClockDelegates().OnTimeChanged.RemoveAll(this);
	}
	ClockSubsystem = nullptr;

	Super::NativeDestruct();
}

