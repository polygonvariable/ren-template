// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InteractEntryUI.h"

// Engine Headers
#include "Components/TextBlock.h"

// Project Headers
#include "Definition/InteractItem.h"
#include "Widget/InteractEntry.h"


void UInteractEntryUI::UpdateDetails()
{
	UInteractEntry* Entry = GetListItem<UInteractEntry>();
	if (IsValid(Entry))
	{
		InteractTitle->SetText(Entry->InteractItem.InteractTitle);
	}
}

void UInteractEntryUI::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	InteractTitle->SetColorAndOpacity(FLinearColor::White);
	UpdateDetails();
}

void UInteractEntryUI::NativeOnItemSelectionChanged(bool bSelected)
{
	if (bSelected)
	{
		InteractTitle->SetColorAndOpacity(FLinearColor::Yellow);
	}
	else
	{
		InteractTitle->SetColorAndOpacity(FLinearColor::White);
	}
}

