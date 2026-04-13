// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/InventoryInstanceSubsystem.h"

// Engine Headers
#include "Manager/RAssetManager.h"

// Project Headers
#include "Library/InventoryPrimaryAsset.h"
#include "Item/InventoryInstance.h"
#include "Delegate/GameLifecycleDelegate.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Subsystem/InventorySubsystem.h"




UInventoryInstance* UInventoryInstanceSubsystem::GetItem(const FGuid& InventoryId, const FPrimaryAssetId& AssetId)
{
	FInventoryInstanceId InstanceId(InventoryId, AssetId);
	TObjectPtr<UInventoryInstance>* FoundItem = InventoryInstances.Find(InstanceId);
	if (FoundItem)
	{
		return FoundItem->Get();
	}

	return CreateItem_Internal(InventoryId, AssetId);
}

bool UInventoryInstanceSubsystem::RemoveItem(const FGuid& InventoryId, const FPrimaryAssetId& AssetId)
{
	FInventoryInstanceId InstanceId(InventoryId, AssetId);
	TObjectPtr<UInventoryInstance>* FoundItem = InventoryInstances.Find(InstanceId);
	if (!FoundItem)
	{
		return false;
	}

	UInventorySubsystem* InventorySubsystem = WeakInventory.Get();
	UInventoryInstance* Item = FoundItem->Get();
	bool bResult = CommitItem_Internal(Item, InventorySubsystem);
	if (!bResult)
	{
		return false;
	}

	InventoryInstances.Remove(InstanceId);

	Item->ResetItem();
	Item->MarkAsGarbage();

	return true;
}

void UInventoryInstanceSubsystem::CommitAllItems()
{
	UInventorySubsystem* InventorySubsystem = WeakInventory.Get();
	for (const TPair<FInventoryInstanceId, UInventoryInstance*>& Pair : InventoryInstances)
	{
		CommitItem_Internal(Pair.Value, InventorySubsystem);
	}
}

bool UInventoryInstanceSubsystem::CommitItem(const FGuid& InventoryId, const FPrimaryAssetId& AssetId)
{
	FInventoryInstanceId InstanceId(InventoryId, AssetId);
	TObjectPtr<UInventoryInstance>* FoundItem = InventoryInstances.Find(InstanceId);
	if (!FoundItem)
	{
		return false;
	}

	UInventorySubsystem* InventorySubsystem = WeakInventory.Get();
	UInventoryInstance* Item = FoundItem->Get();

	bool bResult = CommitItem_Internal(Item, InventorySubsystem);
	return bResult;
}


UInventoryInstance* UInventoryInstanceSubsystem::CreateItem_Internal(const FGuid& InventoryId, const FPrimaryAssetId& AssetId)
{
	return nullptr;
	//UInventorySubsystem* InventorySubsystem = WeakInventory.Get();
	//if (!IsValid(AssetManager) || !IsValid(InventorySubsystem))
	//{
	//	return nullptr;
	//}

	//FAssetData AssetData;
	//AssetManager->GetPrimaryAssetData(AssetId, AssetData);

	//bool bPersistWhenEmpty = false;
	//FInventoryPrimaryAsset::GetPersistWhenEmpty(AssetData, bPersistWhenEmpty);

	//bool bStackable = false;
	//FInventoryPrimaryAsset::GetStackable(AssetData, bStackable);

	//if (!bPersistWhenEmpty || !bStackable)
	//{
	//	LOG_ERROR(LogInventoryInstance, TEXT("Item instance cannot be created for non-persistent item or non-stackable item"));
	//	return nullptr;
	//}

	//const FInventoryInstance* Item = InventorySubsystem->GetItem(InventoryId, AssetId);
	//if (!Item)
	//{
	//	LOG_ERROR(LogInventoryInstance, TEXT("Unable to find item"));
	//	return nullptr;
	//}

	//UInventoryInstance* NewInstance = NewObject<UInventoryInstance>();
	//if (!IsValid(NewInstance))
	//{
	//	LOG_ERROR(LogInventoryInstance, TEXT("Failed to create item instance"));
	//	return nullptr;
	//}

	//NewInstance->InitializeItem(InventoryId, AssetId, (*Item));

	//FInventoryInstanceId InstanceId(InventoryId, AssetId);
	//InventoryInstances.Add(InstanceId, NewInstance);

	//return NewInstance;
}

bool UInventoryInstanceSubsystem::CommitItem_Internal(UInventoryInstance* Item, UInventorySubsystem* InventorySubsystem)
{
	return false;
	//if (!IsValid(Item) || !IsValid(InventorySubsystem))
	//{
	//	return false;
	//}

	//FGuid InventoryId = Item->GetInventoryId();
	//FPrimaryAssetId AssetId = Item->GetAssetId();
	//FGuid ItemId = Item->GetItemId();

	//int Quantity = Item->GetQuantity();

	//bool bResult = InventorySubsystem->UpdateItemById(InventoryId, AssetId, ItemId,
	//	[Quantity](FInventoryInstance* Item)
	//	{
	//		if (Item)
	//		{
	//			Item->Quantity = Quantity;
	//			Item->Sanitize();
	//		}
	//	}
	//);

	//return bResult;
}


void UInventoryInstanceSubsystem::OnPreGameInitialized()
{
	FGameLifecycleDelegate::OnPreGameInitialized.RemoveAll(this);

	UInventorySubsystem* Inventory = UInventorySubsystem::Get(GetGameInstance());
	WeakInventory = TWeakObjectPtr<UInventorySubsystem>(Inventory);

	AssetManager = URAssetManager::GetIfInitialized();
}

bool UInventoryInstanceSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UInventoryInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FGameLifecycleDelegate::OnPreGameInitialized.AddUObject(this, &UInventoryInstanceSubsystem::OnPreGameInitialized);

	LOG_WARNING(LogInventoryInstance, TEXT("InventoryInstanceSubsystem initialized"));
}

void UInventoryInstanceSubsystem::Deinitialize()
{
	FGameLifecycleDelegate::OnPreGameInitialized.RemoveAll(this);

	CommitAllItems();
	InventoryInstances.Empty();

	AssetManager = nullptr;
	WeakInventory.Reset();

	LOG_WARNING(LogInventoryInstance, TEXT("InventoryInstanceSubsystem deinitialized"));
	Super::Deinitialize();
}


UInventoryInstanceSubsystem* UInventoryInstanceSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UInventoryInstanceSubsystem* UInventoryInstanceSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UInventoryInstanceSubsystem>();
}

