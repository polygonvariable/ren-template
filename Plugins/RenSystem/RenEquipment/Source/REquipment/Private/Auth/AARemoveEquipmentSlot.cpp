// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Auth/AARemoveEquipmentSlot.h"

// Project Headers
#include "Delegate/GameUIDelegate.h"
#include "Storage/EquipmentStorageManager.h"
#include "Subsystem/EquipmentSubsystem.h"


void UAARemoveEquipmentSlot::OnStarted()
{
	FGameUIDelegate::OnUIActionStarted.Broadcast();
	EquipmentSubsystem = UEquipmentSubsystem::Get(GetWorld());

	Step_RemoveEquipment();
}

void UAARemoveEquipmentSlot::OnCompleted(bool bSuccess)
{
	FGameUIDelegate::OnUIActionCompleted.Broadcast();
}

void UAARemoveEquipmentSlot::OnCleanup()
{
	EquipmentSubsystem = nullptr;
	Slot = FGameplayTag::EmptyTag;
	OwnerInstanceId.Invalidate();
}

void UAARemoveEquipmentSlot::Step_RemoveEquipment()
{
	if (!IsValid(EquipmentSubsystem))
	{
		Fail(TEXT("Invalid Asset Id"));
		return;
	}

	UEquipmentStorageManager* StorageManager = EquipmentSubsystem->GetStorageManager();
	if (!IsValid(StorageManager))
	{
		Fail(TEXT("Storage Manager is invalid"));
		return;
	}

	if (!StorageManager->RemoveEquipmentFromSlot(OwnerInstanceId, Slot))
	{
		Fail(TEXT("Failed to remove equipment from slot"));
		return;
	}

	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(TimerHandle, this, &UAARemoveEquipmentSlot::Step_Finalize, 0.5f, false);
}

void UAARemoveEquipmentSlot::Step_Finalize()
{
	UWorld* World = GetWorld();
	World->GetTimerManager().ClearTimer(TimerHandle);

	Success();
}

