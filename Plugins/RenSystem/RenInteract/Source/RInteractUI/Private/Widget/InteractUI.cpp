// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InteractUI.h"

// Engine Headers
#include "Components/ListView.h"

// Project Headers
#include "Definition/InteractItem.h"
#include "Library/PoolHelper.h"
#include "Log/LogMacro.h"
#include "Subsystem/InteractSubsystem.h"
#include "Widget/InteractEntry.h"
#include "Widget/InteractEntryUI.h"


void UInteractUI::ScrollSelection(int Direction)
{
	int TotalItems = InteractList->GetNumItems();
	if (TotalItems <= 0)
	{
		return;
	}

	UObject* SelectedItem = InteractList->GetSelectedItem();
	int SelectedIndex = InteractList->GetIndexForItem(SelectedItem);

	SelectedIndex = FMath::Clamp(SelectedIndex + Direction, 0, TotalItems - 1);
	InteractList->SetSelectedIndex(SelectedIndex);
}

void UInteractUI::HandleInteractAdded(const FGuid& InteractId, const FInteractItem& InteractItem)
{
	TObjectPtr<UInteractEntry>* FoundEntry = InteractEntries.Find(InteractId);
	if (FoundEntry)
	{
		UInteractEntry* Entry = FoundEntry->Get();
		if (IsValid(Entry))
		{
			UInteractEntryUI* Widget = InteractList->GetEntryWidgetFromItem<UInteractEntryUI>(Entry);
			if (IsValid(Widget))
			{
				Entry->InteractItem = InteractItem;
				Widget->UpdateDetails();
			}
		}
		return;
	}

	UInteractEntry* Entry = FPoolHelper::AcquireFromArray<UInteractEntry>(_InteractPool, UInteractEntry::StaticClass(), this);
	if (!IsValid(Entry))
	{
		LOG_ERROR(LogTemp, TEXT("InteractEntry is invalid"));
		return;
	}

	Entry->InteractId = InteractId;
	Entry->InteractItem = InteractItem;

	InteractList->AddItem(Entry);
	InteractEntries.Add(InteractId, Entry);

	UObject* SelectedItem = InteractList->GetSelectedItem();
	if (!IsValid(SelectedItem))
	{
		InteractList->SetSelectedIndex(0);
	}
}

void UInteractUI::HandleInteractRemoved(const FGuid& InteractId)
{
	TObjectPtr<UInteractEntry>* FoundEntry = InteractEntries.Find(InteractId);
	if (FoundEntry)
	{
		UInteractEntry* Entry = FoundEntry->Get();
		if (Entry)
		{
			if (Entry == InteractList->GetSelectedItem())
			{
				InteractList->ClearSelection();
			}

			Entry->ResetData();

			FPoolHelper::ReturnToArray<UInteractEntry>(_InteractPool, Entry);
			InteractList->RemoveItem(Entry);
		}
	}

	InteractEntries.Remove(InteractId);

	UObject* SelectedItem = InteractList->GetSelectedItem();
	if (!IsValid(SelectedItem))
	{
		InteractList->SetSelectedIndex(0);
	}
}

void UInteractUI::NativeConstruct()
{
	InteractSubsystem = UInteractSubsystem::Get(GetWorld());
	if (IsValid(InteractSubsystem))
	{
		InteractSubsystem->OnInteractAdded.BindUObject(this, &UInteractUI::HandleInteractAdded);
		InteractSubsystem->OnInteractRemoved.BindUObject(this, &UInteractUI::HandleInteractRemoved);
	}

	Super::NativeConstruct();
}

void UInteractUI::NativeDestruct()
{
	if (IsValid(InteractSubsystem))
	{
		InteractSubsystem->OnInteractAdded.Unbind();
		InteractSubsystem->OnInteractRemoved.Unbind();
	}
	InteractSubsystem = nullptr;

	InteractEntries.Empty();
	InteractList->ClearListItems();

	Super::NativeDestruct();
}

