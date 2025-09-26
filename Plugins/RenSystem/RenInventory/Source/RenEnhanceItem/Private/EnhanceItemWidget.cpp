// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EnhanceItemWidget.h"

// Engine Headers
#include "Components/WidgetSwitcher.h"
#include "InstancedStruct.h"

// Project Headers
#include "RCoreEnhance/Public/EnhanceLibrary.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenAsset/Public/Inventory/Asset/Category/EnhanceableAsset.h"
#include "RenAsset/Public/Inventory/Asset/Type/EnhanceAsset.h"
#include "RenAsset/Public/Inventory/Type/InventoryAssetQuantity.h"

#include "RenInventory/Public/InventoryDefinition.h"
#include "RenInventory/Public/InventorySubsystem.h"
#include "RenInventory/Public/Widget/InventoryCollectionWidget.h"
#include "RenInventory/Public/Widget/InventoryDetailWidget.h"
#include "RenInventory/Public/Widget/InventoryEntryObject.h"

#include "RenEnhanceItem/Public/EnhanceItemSubsystem.h"



//void UEnhanceItemWidget::InitializeDetails(const FName& ItemGuid, const FInventoryRecord* Record, UInventoryAsset* Asset)
//{
//	ActiveItemGuid = ItemGuid;
//
//	UEnhanceableAsset* ItemAsset = Cast<UEnhanceableAsset>(Asset);
//	if (ItemAsset)
//	{
//		ActiveAsset = ItemAsset;
//
//		HandleLevelUpDisplay();
//		HandleRankUpDisplay(Record);
//	}
//
//	if (InventoryDetail)
//	{
//		InventoryDetail->InitializeDetails(ItemGuid, Record, Asset);
//	}
//}

void UEnhanceItemWidget::ResetDetails()
{
	ActiveItemGuid = NAME_None;
	ActiveAsset = nullptr;
}

void UEnhanceItemWidget::RefreshDetails()
{
	/*UInventorySubsystem* Subsystem = InventorySubsystem.Get();
	if (!IsValid(Subsystem)) {
		LOG_ERROR(LogTemp, "InventorySubsystem is invalid");
		return;
	}

	const FInventoryRecord* Record = Subsystem->GetItemRecord(ContainerId, ActiveItemGuid);
	if (!Record)
	{
		LOG_ERROR(LogTemp, "Record is invalid");
		return;
	}

	HandleRankUpDisplay(Record);*/
}

void UEnhanceItemWidget::TryLevelUp(FName ItemGuid)
{
	/*UEnhanceItemSubsystem* Subsystem = EnhanceItemSubsystem.Get();
	if (IsValid(Subsystem) && ItemGuid.IsValid())
	{
		Subsystem->LevelUpItem(ContainerId, ActiveItemGuid, ItemGuid);
	}*/
}

void UEnhanceItemWidget::TryRankUp()
{
	/*UEnhanceItemSubsystem* Subsystem = EnhanceItemSubsystem.Get();
	if (IsValid(Subsystem) && ActiveItemGuid.IsValid())
	{
		Subsystem->RankUpItem(ContainerId, ActiveItemGuid);
	}*/
}

void UEnhanceItemWidget::HandleLevelUpDisplay()
{
	if (LevelUpCollection)
	{
		/*TArray<FName> ItemIds;
		const TArray<TObjectPtr<UEnhanceAsset>>& CostItems = ActiveAsset->EnhanceCosts;

		for (UEnhanceAsset* EnhanceAsset : CostItems)
		{
			ItemIds.Add(EnhanceAsset->ItemId);
		}

		LevelUpCollection->FilterRule.FilterId.Included.Append(ItemIds);
		LevelUpCollection->DisplayItems();*/
	}
}

void UEnhanceItemWidget::HandleRankUpDisplay(const FInventoryRecord* Record)
{
	/*bool bCanRankUp = UEnhanceLibrary::CanRankUp(
		Record->EnhanceRecord.Experience,
		Record->EnhanceRecord.Level,
		Record->EnhanceRecord.Rank,
		ActiveAsset->GetLevelInterval(Record->EnhanceRecord.Rank)
	);
	if (EnhanceSwitcher) EnhanceSwitcher->SetActiveWidgetIndex(bCanRankUp ? 1 : 0);

	if (RankUpCollection)
	{
		RankUpCollection->ClearItems();

		const TArray<FInventoryAssetQuantity>& RankItems = ActiveAsset->RankingCosts;
		if (bCanRankUp && RankItems.IsValidIndex(Record->EnhanceRecord.Rank))
		{
			const FInventoryAssetQuantity& RankItem = RankItems[Record->EnhanceRecord.Rank];
			const TMap<FName, int>& ItemQuantities = RankItem.GetItemIds();

			TMap<FName, FInstancedStruct> Payloads;
			for (auto& Pair : ItemQuantities)
			{
				FInventoryPayloadQuantity Payload;
				Payload.Quantity = Pair.Value;

				Payloads.Add(Pair.Key, FInstancedStruct::Make(Payload));
			}

			RankUpCollection->SetPayloads(Payloads);

			TArray<FName> RankItemIds;
			ItemQuantities.GetKeys(RankItemIds);

			RankUpCollection->FilterRule.FilterId.Included.Append(RankItemIds);
			RankUpCollection->DisplayItems();
		}
	}*/
}

void UEnhanceItemWidget::HandleItemSelected(FName ItemGuid, const FInventoryRecord* Record, UInventoryAsset* Asset)
{
	TryLevelUp(ItemGuid);
}

void UEnhanceItemWidget::NativeConstruct()
{
	/*UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		UEnhanceItemSubsystem* EnhanceItemSubsystemPtr = GetGameInstance()->GetSubsystem<UEnhanceItemSubsystem>();
		if (IsValid(EnhanceItemSubsystemPtr))
		{
			EnhanceItemSubsystem = EnhanceItemSubsystemPtr;
		}

		UInventorySubsystem* InventorySubsystemPtr = GameInstance->GetSubsystem<UInventorySubsystem>();
		if (!IsValid(InventorySubsystemPtr))
		{
			LOG_ERROR(LogTemp, TEXT("InventorySubsystem is invalid"));
			return;
		}

		if (bAutoRefresh)
		{
			InventorySubsystemPtr->OnItemAdded.AddWeakLambda(this, [this](FName ContainerId, FName ItemGuid, const FInventoryRecord* Record)	{ if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
			InventorySubsystemPtr->OnItemRemoved.AddWeakLambda(this, [this](FName ContainerId, FName ItemGuid, FInventoryRecord Record)			{ if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
			InventorySubsystemPtr->OnItemUpdated.AddWeakLambda(this, [this](FName ContainerId, FName ItemGuid, const FInventoryRecord* Record)	{ if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
		}

		InventorySubsystem = InventorySubsystemPtr;
	}

	if (LevelUpCollection)
	{
		LevelUpCollection->OnItemSelected.AddUObject(this, &UEnhanceItemWidget::HandleItemSelected);
	}*/

	Super::NativeConstruct();
}

void UEnhanceItemWidget::NativeDestruct()
{
	/*ResetDetails();

	if (LevelUpCollection)
	{
		LevelUpCollection->OnItemSelected.RemoveAll(this);
	}

	UInventorySubsystem* InventorySubsystemPtr = InventorySubsystem.Get();
	if (IsValid(InventorySubsystemPtr))
	{
		InventorySubsystemPtr->OnItemAdded.RemoveAll(this);
		InventorySubsystemPtr->OnItemRemoved.RemoveAll(this);
		InventorySubsystemPtr->OnItemUpdated.RemoveAll(this);
	}
	InventorySubsystem.Reset();

	EnhanceItemSubsystem.Reset();*/

	Super::NativeDestruct();
}

