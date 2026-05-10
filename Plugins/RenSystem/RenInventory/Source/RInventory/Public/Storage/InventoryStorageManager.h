// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Interface/AscensionInstanceProvider.h"
#include "Interface/AssetInstanceCollection.h"
#include "Interface/StorageManager.h"

// Generated Headers
#include "InventoryStorageManager.generated.h"

// Module Macros
#define REN_API RINVENTORY_API

// Forward Declarations
class UAssetManager;
class UInventoryStorage;
class UFilterCriterion;
struct FInventoryQueryRule;
struct FInventorySortEntry;
struct FInventoryStack;
struct FInventoryInstance;


/**
 *
 */
UCLASS(MinimalAPI)
class UInventoryStorageManager : public UObject, public IStorageManager, public IAssetInstanceCollection, public IAscensionInstanceProvider
{

	GENERATED_BODY()

public:

	FGameEventDelegate OnStorageUpdated;


	REN_API bool UpdateInstance(const FPrimaryAssetId& AssetId, TFunctionRef<void(FInventoryInstance*)> Callback);
	REN_API bool UpdateInstanceById(const FPrimaryAssetId& AssetId, const FGuid& InstanceId, TFunctionRef<void(FInventoryInstance*)> Callback);

	REN_API int GetTotalQuantity(const FPrimaryAssetId& AssetId) const;
	REN_API const FInventoryInstance* GetInstance(const FPrimaryAssetId& AssetId) const;
	REN_API const FInventoryInstance* GetInstanceById(const FPrimaryAssetId& AssetId, const FGuid& InstanceId) const;

	REN_API void QueryInstances(const UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TArray<FInventorySortEntry>& OutSortedItems);

	// ~ IAssetInstanceCollection
	REN_API virtual bool AddInstance(const FPrimaryAssetId& AssetId, int Quantity) override;
	REN_API virtual bool AddInstances(const TMap<FPrimaryAssetId, int>& AssetIds, int Multiplier) override;

	REN_API virtual bool RemoveInstance(const FPrimaryAssetId& AssetId, int Quantity) override;
	REN_API virtual bool RemoveInstances(const TMap<FPrimaryAssetId, int>& AssetIds, int Multiplier) override;

	REN_API virtual bool RemoveAnyInstances(const TMap<FPrimaryAssetId, int>& InAssetIds, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity) override;
	REN_API virtual bool RemoveInstanceById(const FPrimaryAssetId& AssetId, const FGuid& InstanceId, int Quantity) override;

	REN_API virtual bool ContainInstance(const FPrimaryAssetId& AssetId, int Quantity) const override;
	REN_API virtual bool ContainInstances(const TMap<FPrimaryAssetId, int>& AssetIds, int Multiplier) const override;
	REN_API virtual bool ContainAnyInstances(const TMap<FPrimaryAssetId, int>& InAssetIds, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity) const override;

	virtual FGameEventDelegate& GetOnAssetInstanceCollectionUpdated() override;
	// ~ End of IAssetInstanceCollection

	// ~ IAscensionInstanceProvider
	virtual const FAscensionData* GetAscensionInstance(const FPrimaryAssetId& AssetId, const FGuid& InstanceId) const override;
	// ~ End of IAscensionInstanceProvider

	// ~ UStorageManager
	virtual UObject* GetStorage() const override;
	virtual void SetStorage(UObject* Storage) override;
	virtual void OnStorageLoaded(bool bIsNew) override;
	virtual FGameEventDelegate& GetOnStorageUpdated() override;
	// ~ End of UStorageManager

protected:

	FInventoryInstance* GetMutableItemByIndex(const FPrimaryAssetId& AssetId, int Index);
	FInventoryInstance* GetMutableItemById(const FPrimaryAssetId& AssetId, const FGuid& ItemId);

	void HandleItemSorting(TArray<FInventorySortEntry>& SortedItems, const FInventoryQueryRule& QueryRule) const;
	void QueryAssetItems(UAssetManager* AssetManager, const UFilterCriterion* FilterCriterion, TArray<FInventorySortEntry>& OutSortedItems) const;
	void QueryInstanceItems(UAssetManager* AssetManager, const UFilterCriterion* FilterCriterion, TArray<FInventorySortEntry>& OutSortedItems) const;

	const FInventoryStack* GetStack(const FPrimaryAssetId& AssetId) const;
	FInventoryStack* FindOrAddStack(const FPrimaryAssetId& AssetId);

	int GetItemQuantity_Internal(const FPrimaryAssetId& AssetId) const;

	bool AddItem_Internal(const FPrimaryAssetId& AssetId, int Quantity, FInventoryStack* Stack);
	void AddItem_InternalCreate(const FPrimaryAssetId& AssetId, TArray<FInventoryInstance>& ItemList, int Quantity);
	void AddItem_InternalUpdate(const FPrimaryAssetId& AssetId, FInventoryInstance& Record, int Quantity);

	bool RemoveItem_Internal(const FPrimaryAssetId& AssetId, int Quantity, FInventoryStack* Stack);
	bool RemoveItemById_Internal(const FPrimaryAssetId& AssetId, const FGuid& ItemId, int Quantity, FInventoryStack* Stack);
	bool RemoveItem_InternalStack(const FPrimaryAssetId& AssetId, TArray<FInventoryInstance>& ItemList, int Quantity);
	bool RemoveItem_InternalRemove(const FPrimaryAssetId& AssetId, TArray<FInventoryInstance>& ItemList, FInventoryInstance* Item);
	void RemoveItem_InternalUpdate(const FPrimaryAssetId& AssetId, TArray<FInventoryInstance>& ItemList, bool bPersistWhenEmpty, FInventoryInstance* Item, int Quantity);

private:

	UPROPERTY()
	TObjectPtr<UInventoryStorage> LocalStorage = nullptr;

};


// Module Macros
#undef REN_API

