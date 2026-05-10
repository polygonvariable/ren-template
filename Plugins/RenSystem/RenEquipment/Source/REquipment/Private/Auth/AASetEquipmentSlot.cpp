// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Auth/AASetEquipmentSlot.h"

// Project Headers
#include "Delegate/GameUIDelegate.h"
#include "Storage/EquipmentStorageManager.h"
#include "Subsystem/EquipmentSubsystem.h"


void UAASetEquipmentSlot::OnStarted()
{
	FGameUIDelegate::OnUIActionStarted.Broadcast();
	EquipmentSubsystem = UEquipmentSubsystem::Get(GetWorld());

	Step_AssignEquipment();
}

void UAASetEquipmentSlot::OnCompleted(bool bSuccess)
{
	FGameUIDelegate::OnUIActionCompleted.Broadcast();
}

void UAASetEquipmentSlot::OnCleanup()
{
	EquipmentSubsystem = nullptr;
	Slot = FGameplayTag::EmptyTag;

	OwnerInstanceId.Invalidate();
	OwnerAssetId = FPrimaryAssetId();

	EquipmentInstanceId.Invalidate();
	EquipmentAssetId = FPrimaryAssetId();
}

void UAASetEquipmentSlot::Step_AssignEquipment()
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

	if (!StorageManager->SetEquipmentAtSlot(OwnerInstanceId, OwnerAssetId, Slot, EquipmentInstanceId, EquipmentAssetId))
	{
		Fail(TEXT("Failed to set equipment at slot"));
		return;
	}

	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(TimerHandle, this, &UAASetEquipmentSlot::Step_Finalize, 0.5f, false);
}

void UAASetEquipmentSlot::Step_Finalize()
{
	UWorld* World = GetWorld();
	World->GetTimerManager().ClearTimer(TimerHandle);

	Success();
}

