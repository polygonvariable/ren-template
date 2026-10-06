// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InteractUI.h"

// Engine Headers
#include "Components/ListView.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"

// Project Headers
#include "Core/PoolLibrary.h"
#include "InteractEntry.h"
#include "InteractEntryUI.h"
#include "InteractItem.h"
#include "InteractSubsystem.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


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
		LOG_ERROR(LogInteract, TEXT("InteractEntry already exists"));
		return;
	}
	
	UInteractEntry* Entry = FPoolLibrary::AcquireFromArray<UInteractEntry>(_InteractPool, UInteractEntry::StaticClass(), this);
	if (!IsValid(Entry))
	{
		LOG_ERROR(LogInteract, TEXT("InteractEntry is invalid"));
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

			FPoolLibrary::ReturnToArray<UInteractEntry>(_InteractPool, Entry);
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


void UInteractUI::HandleOnInteractScroll(const FInputActionValue& Value)
{
	if (Value.GetValueType() == EInputActionValueType::Axis1D)
	{
		FInputActionValue::Axis1D Axis = Value.Get<float>();
		ScrollSelection(Axis);
	}
}

void UInteractUI::HandleOnInteractSelect(const FInputActionValue& Value)
{
	const UInteractEntry* Entry = InteractList->GetSelectedItem<UInteractEntry>();
	if (IsValid(InteractSubsystem) && IsValid(Entry))
	{
		InteractSubsystem->InteractItemById(Entry->InteractId);
	}
}


void UInteractUI::NativeConstruct()
{
	Super::NativeConstruct();

	InteractSubsystem = UInteractSubsystem::Get(GetWorld());
	if (IsValid(InteractSubsystem))
	{
		InteractSubsystem->OnInteractAdded.BindUObject(this, &UInteractUI::HandleInteractAdded);
		InteractSubsystem->OnInteractRemoved.BindUObject(this, &UInteractUI::HandleInteractRemoved);
	}

	APlayerController* PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController))
	{
		UEnhancedInputComponent* ControllerInput = Cast<UEnhancedInputComponent>((PlayerController->InputComponent));
		if (IsValid(ControllerInput))
		{
			if (IsValid(InteractScroll))
			{
				FEnhancedInputActionEventBinding& Binding = ControllerInput->BindAction(InteractScroll, ETriggerEvent::Triggered, this, &UInteractUI::HandleOnInteractScroll);
				InputHandles.Add(Binding.GetHandle());
			}
			if (IsValid(InteractSelect))
			{
				FEnhancedInputActionEventBinding& Binding = ControllerInput->BindAction(InteractSelect, ETriggerEvent::Triggered, this, &UInteractUI::HandleOnInteractSelect);
				InputHandles.Add(Binding.GetHandle());
			}
		}
	}
}

void UInteractUI::NativeDestruct()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController))
	{
		UEnhancedInputComponent* ControllerInput = Cast<UEnhancedInputComponent>((PlayerController->InputComponent));
		if (IsValid(ControllerInput))
		{
			for (const uint8& Handle : InputHandles)
			{
				ControllerInput->RemoveBindingByHandle(Handle);
			}
			InputHandles.Empty();
		}
	}

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

