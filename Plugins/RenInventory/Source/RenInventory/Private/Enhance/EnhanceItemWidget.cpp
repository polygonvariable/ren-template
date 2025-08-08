// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Enhance/EnhanceItemWidget.h"

// Engine Headers

// Project Headers
#include "RenCore/Public/Library/EnhanceLibrary.h"

#include "RenAsset/Public/Inventory/Asset/Type/EnhanceAsset.h"
#include "RenAsset/Public/Inventory/Asset/Category/EnhanceableAsset.h"

#include "RenInventory/Public/Widget/InventoryCollectionWidget.h"
#include "RenInventory/Public/Widget/InventoryDetailWidget.h"

#include "RenInventory/Public/Enhance/EnhanceItemSubsystem.h"



void UEnhanceItemWidget::InitializeDetail(FName ItemGuid, const FInventoryRecord* Record, UInventoryAsset* Asset)
{
	ActiveItemGuid = ItemGuid;

	if (InventoryCollection)
	{
		UEnhanceableAsset* ItemAsset = Cast<UEnhanceableAsset>(Asset);
		if (ItemAsset)
		{
			TArray<FName> ItemIds;
			TSet<TObjectPtr<UEnhanceAsset>>& CostItems = ItemAsset->EnhanceCosts;

			for (UEnhanceAsset* EnhanceAsset : CostItems)
			{
				ItemIds.Add(EnhanceAsset->ItemId);
			}

			InventoryCollection->FilterRule.FilterId.bEnable = true;
			InventoryCollection->FilterRule.FilterId.bEnableStrictMode = true;
			InventoryCollection->FilterRule.FilterId.Included.Append(ItemIds);
		}
		InventoryCollection->DisplayItems();
	}

	if (InventoryDetail)
	{
		InventoryDetail->InitializeDetail(ItemGuid, Record, Asset);
	}
}

void UEnhanceItemWidget::TryLevelUp(FName ItemGuid)
{
	UEnhanceItemSubsystem* Subsystem = EnhanceItemSubsystem.Get();
	if (IsValid(Subsystem))
	{
		Subsystem->LevelUpItem(ContainerId, ActiveItemGuid, ItemGuid);
	}
}

void UEnhanceItemWidget::TryRankUp()
{
	UEnhanceItemSubsystem* Subsystem = EnhanceItemSubsystem.Get();
	if (IsValid(Subsystem))
	{
		Subsystem->RankUpItem(ContainerId, ActiveItemGuid);
	}
}

void UEnhanceItemWidget::HandleItemSelected(FName ItemGuid, const FInventoryRecord* Record, UInventoryAsset* Asset)
{
	TryLevelUp(ItemGuid);
}

void UEnhanceItemWidget::NativeConstruct()
{
	UEnhanceItemSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UEnhanceItemSubsystem>();
	if (IsValid(Subsystem))
	{
		EnhanceItemSubsystem = Subsystem;

		if (InventoryCollection)
		{
			InventoryCollection->OnItemSelected.AddUObject(this, &UEnhanceItemWidget::HandleItemSelected);
		}
	}

	Super::NativeConstruct();
}

void UEnhanceItemWidget::NativeDestruct()
{
	if (InventoryCollection)
	{
		InventoryCollection->OnItemSelected.RemoveAll(this);
	}
	EnhanceItemSubsystem.Reset();

	Super::NativeDestruct();
}

