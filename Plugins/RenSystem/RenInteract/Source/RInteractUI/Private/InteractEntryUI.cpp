// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InteractEntryUI.h"

// Engine Headers
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"

// Project Headers
#include "InteractEntry.h"
#include "InteractItem.h"
#include "InteractSubsystem.h"


void UInteractEntryUI::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	NativeOnItemSelectionChanged(false);

	UInteractEntry* Entry = GetListItem<UInteractEntry>();
	if (IsValid(Entry))
	{
		InteractTitle->SetText(Entry->InteractItem.InteractTitle);
	}
}

void UInteractEntryUI::NativeOnItemSelectionChanged(bool bSelected)
{
	FButtonStyle ButtonStyle = InteractButton->GetStyle();
	if (bSelected)
	{
		ButtonStyle.NormalForeground = _ButtonHoverForeground;

		InteractButton->SetStyle(ButtonStyle);
		InteractTitle->SetColorAndOpacity(_ButtonHoverForeground);
	}
	else
	{
		ButtonStyle.NormalForeground = _ButtonNormalForeground;

		InteractButton->SetStyle(ButtonStyle);
		InteractTitle->SetColorAndOpacity(_TextNormal);
	}
}

void UInteractEntryUI::HandleOnButtonClicked()
{
	UInteractSubsystem* InteractSubsystem = UInteractSubsystem::Get(GetWorld());
	const UInteractEntry* Entry = GetListItem<UInteractEntry>();
	if (IsValid(InteractSubsystem) && IsValid(Entry))
	{
		InteractSubsystem->InteractItemById(Entry->InteractId);
	}
}

void UInteractEntryUI::NativeConstruct()
{
	Super::NativeConstruct();

	InteractButton->OnClicked.AddDynamic(this, &UInteractEntryUI::HandleOnButtonClicked);

	_TextNormal = InteractTitle->GetColorAndOpacity();

	const FButtonStyle& Style = InteractButton->GetStyle();
	_ButtonNormalForeground = Style.NormalForeground;
	_ButtonHoverForeground = Style.HoveredForeground;
}

void UInteractEntryUI::NativeDestruct()
{
	InteractButton->OnClicked.Clear();

	Super::NativeDestruct();
}

