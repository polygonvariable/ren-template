// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Storage/ShopStorageManager.h"

// Project Headers
#include "Storage/ShopStorage.h"


const FShopData* UShopStorageManager::GetItem(const FTradeKey& TradeKey) const
{
	if (!IsValid(LocalStorage))
	{
		return nullptr;
	}
	return LocalStorage->ShopItems.Find(TradeKey);
}

bool UShopStorageManager::AddItem(const FTradeKey& TradeKey)
{
	if (!IsValid(LocalStorage))
	{
		return false;
	}

	TMap<FTradeKey, FShopData>& ShopItems = LocalStorage->ShopItems;
	FShopData* FoundData = ShopItems.Find(TradeKey);
	if (!FoundData)
	{
		ShopItems.Add(TradeKey, FShopData(1));
	}
	else
	{
		FoundData->PurchaseCount++;
	}

	OnStorageUpdated.Broadcast();
	return true;
}

void UShopStorageManager::ResetItems()
{
	if (!IsValid(LocalStorage))
	{
		return;
	}
	LocalStorage->ShopItems.Empty();
	OnStorageUpdated.Broadcast();
}

UObject* UShopStorageManager::GetStorage() const
{
	return LocalStorage;
}

void UShopStorageManager::SetStorage(UObject* Storage)
{
	LocalStorage = Cast<UShopStorage>(Storage);
}

FGameEventDelegate& UShopStorageManager::GetOnStorageUpdated()
{
	return OnStorageUpdated;
}

