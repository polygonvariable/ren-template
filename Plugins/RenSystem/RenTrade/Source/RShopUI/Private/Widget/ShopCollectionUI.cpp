// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/ShopCollectionUI.h"

// Project Headers
#include "Storage/ShopStorageManager.h"
#include "Subsystem/ShopSubsystem.h"
#include "Widget/TradeEntry.h"


void UShopCollectionUI::DisplayEntries()
{
	if (!IsValid(ShopSubsystem))
	{
		return;
	}

	ShopSubsystem->QueryItems(TradeAsset, TradeCollectionId,
		[this](const FPrimaryAssetId& ItemAssetId, const FAssetDetail_Trade& ItemDetail)
		{
			UTradeEntry* Entry = GetEntryFromPool<UTradeEntry>();
			if (IsValid(Entry))
			{
				Entry->TradeDetail = ItemDetail;
				AddEntry(ItemAssetId, Entry);
			}
		}
	);
}

void UShopCollectionUI::NativeConstruct()
{
	ShopSubsystem = UShopSubsystem::Get(GetGameInstance());
	if (IsValid(ShopSubsystem))
	{
		StorageManager = ShopSubsystem->GetStorageManager();
		if (IsValid(StorageManager))
		{
			StorageManager->OnStorageUpdated.AddUObject(this, &UShopCollectionUI::RefreshEntries);
		}
	}

	Super::NativeConstruct();
}

void UShopCollectionUI::NativeDestruct()
{
	if (IsValid(StorageManager))
	{
		StorageManager->OnStorageUpdated.RemoveAll(this);
	}
	StorageManager = nullptr;
	ShopSubsystem = nullptr;

	Super::NativeDestruct();
}

