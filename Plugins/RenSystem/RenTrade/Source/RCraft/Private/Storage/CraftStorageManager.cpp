// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Storage/CraftStorageManager.h"

// Project Headers
#include "Storage/CraftStorage.h"


const FCraftData* UCraftStorageManager::GetItem(const FTradeKey& TradeKey) const
{
	if (!IsValid(LocalStorage))
	{
		return nullptr;
	}
	const FCraftData* FoundData = LocalStorage->CraftItems.Find(TradeKey);
	return FoundData;
}

bool UCraftStorageManager::AddItem(const FTradeKey& TradeKey, FTimespan BatchProcessingTime)
{
	if (!IsValid(LocalStorage))
	{
		return false;
	}

	TMap<FTradeKey, FCraftData>& CraftItems = LocalStorage->CraftItems;
	FCraftData* FoundData = CraftItems.Find(TradeKey);
	if (FoundData)
	{
		FoundData->PendingQuantity++;
	}
	else
	{
		CraftItems.Add(TradeKey, FCraftData(1, FDateTime::Now(), BatchProcessingTime));
	}

	OnStorageUpdated.Broadcast();
	return true;
}

void UCraftStorageManager::ResetItems()
{
	if (IsValid(LocalStorage))
	{
		LocalStorage->CraftItems.Empty();
		OnStorageUpdated.Broadcast();
	}
}

int UCraftStorageManager::ClaimCraftedItems(const FTradeKey& TradeKey)
{
	if (!IsValid(LocalStorage))
	{
		return 0;
	}

	TMap<FTradeKey, FCraftData>& CraftItems = LocalStorage->CraftItems;
	FCraftData* FoundData = CraftItems.Find(TradeKey);
	if (!FoundData)
	{
		return 0;
	}

	int TotalQuantity = FoundData->PendingQuantity;
	FDateTime BatchTime = FoundData->BatchStartTimestamp;
	FTimespan BatchDuration = FoundData->BatchProcessingTime;

	FDateTime CurrentTime = FDateTime::Now();
	FTimespan Elapsed = CurrentTime - BatchTime;

	int Completed = 0;
	if (BatchDuration.GetTicks() > 0)
	{
		Completed = Elapsed.GetTicks() / BatchDuration.GetTicks();
	}

	int AvailableQuantity = FMath::Clamp(Completed, 0, TotalQuantity);
	if (AvailableQuantity > 0)
	{
		FoundData->BatchStartTimestamp = BatchTime + (BatchDuration * AvailableQuantity);
	}

	FoundData->PendingQuantity -= AvailableQuantity;
	FoundData->Sanitize();

	if (FoundData->PendingQuantity <= 0)
	{
		CraftItems.Remove(TradeKey);
	}

	if (AvailableQuantity > 0)
	{
		OnStorageUpdated.Broadcast();
	}

	return AvailableQuantity;
}

UObject* UCraftStorageManager::GetStorage() const
{
	return LocalStorage;
}

void UCraftStorageManager::SetStorage(UObject* Storage)
{
	LocalStorage = Cast<UCraftStorage>(Storage);
}

FGameEventDelegate& UCraftStorageManager::GetOnStorageUpdated()
{
	return OnStorageUpdated;
}

