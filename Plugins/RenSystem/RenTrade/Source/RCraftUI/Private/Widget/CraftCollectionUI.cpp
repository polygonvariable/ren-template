// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/CraftCollectionUI.h"

// Project Headers
#include "Asset/TradeAsset.h"
#include "System/CraftStorageManager.h"
#include "System/CraftSubsystem.h"
#include "Widget/CraftEntry.h"


void UCraftCollectionUI::DisplayEntries()
{
	if (!IsValid(CraftSubsystem))
	{
		return;
	}

	FPrimaryAssetId TradeAssetId = TradeAsset->GetPrimaryAssetId();

	CraftSubsystem->QueryItems(TradeAsset, TradeCollectionId, QuerySource,
		[this, TradeAssetId](const FPrimaryAssetId& ItemAssetId, const FAssetDetail_Trade& ItemDetail, const FCraftInstance* CraftData) {

			UCraftEntry* Entry = GetEntryFromPool<UCraftEntry>();
			if (IsValid(Entry))
			{
				Entry->CraftData = (CraftData) ? *CraftData : FCraftInstance();
				Entry->TradeDetail = ItemDetail;
				AddEntry(ItemAssetId, Entry);
			}

		}
	);
}

void UCraftCollectionUI::NativeConstruct()
{
	CraftSubsystem = UCraftSubsystem::Get(GetGameInstance());
	if (IsValid(CraftSubsystem))
	{
		StorageManager = CraftSubsystem->GetStorageManager();
		if (IsValid(StorageManager))
		{
			StorageManager->OnStorageUpdated.AddUObject(this, &UCraftCollectionUI::RefreshEntries);
		}
	}

	Super::NativeConstruct();
}

void UCraftCollectionUI::NativeDestruct()
{
	if (IsValid(StorageManager))
	{
		StorageManager->OnStorageUpdated.RemoveAll(this);
	}
	StorageManager = nullptr;
	CraftSubsystem = nullptr;

	Super::NativeDestruct();
}

