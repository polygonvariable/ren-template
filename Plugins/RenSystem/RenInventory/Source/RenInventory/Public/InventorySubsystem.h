// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers

// Generated Headers
#include "InventorySubsystem.generated.h"

// Forward Declarations
class UAssetManager;

class UFilterCriterion;
class UInventoryAsset;
class IInventoryProviderInterface;

struct FInventoryRecord;
struct FInventoryStack;
struct FInventoryQueryRule;
struct FInventorySortEntry;



/**
 *
 */
UCLASS(MinimalAPI)
class UInventorySubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool AddItem(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity);
	RENINVENTORY_API bool AddItems(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items);

	/*
	 * Removes an item from the stack within the specified container.
	 * Useful when the exact item ID is not required.
	 */
	RENINVENTORY_API bool RemoveItem(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity);

	/*
	 * Removes an item from the stack within the specified container.
	 * Useful when the exact item ID is not required.
	 */
	RENINVENTORY_API bool RemoveItems(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items);
	RENINVENTORY_API bool RemoveItems(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items, int Multiplier);
	RENINVENTORY_API bool RemoveItemById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId, int Quantity);

	RENINVENTORY_API bool ContainsItem(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity) const;
	RENINVENTORY_API bool ContainsItems(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items) const;
	RENINVENTORY_API bool ContainsItems(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items, int Multiplier) const;
	RENINVENTORY_API bool ContainsItemById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId, int Quantity) const;

	RENINVENTORY_API int GetMaxMultiplier(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items) const;

	RENINVENTORY_API bool UpdateItem(FName ContainerId, const FPrimaryAssetId& AssetId, TFunctionRef<bool(FInventoryRecord*)> Callback);
	RENINVENTORY_API bool UpdateItemById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId, TFunctionRef<bool(FInventoryRecord*)> Callback);

	RENINVENTORY_API int GetStackCount(FName ContainerId, const FPrimaryAssetId& AssetId) const;
	int GetStackCount(FName ContainerId, const FPrimaryAssetId& AssetId, IInventoryProviderInterface* InventoryInterface) const;

	RENINVENTORY_API int GetTotalQuantity(FName ContainerId, const FPrimaryAssetId& AssetId) const;
	int GetTotalQuantity(FName ContainerId, const FPrimaryAssetId& AssetId, IInventoryProviderInterface* InventoryInterface) const;

	RENINVENTORY_API int GetQuantityById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId) const;
	int GetQuantityById(FName ContainerId, const FPrimaryAssetId& AssetId, IInventoryProviderInterface* InventoryInterface, FName RecordId) const;

	const FInventoryStack* GetInventoryStack(FName ContainerId, const FPrimaryAssetId& AssetId) const;
	const FInventoryStack* GetInventoryStack(FName ContainerId, const FPrimaryAssetId& AssetId, IInventoryProviderInterface* InventoryInterface) const;

	const TArray<FInventoryRecord>* GetRecords(FName ContainerId, const FPrimaryAssetId& AssetId) const;
	const TArray<FInventoryRecord>* GetRecords(FName ContainerId, const FPrimaryAssetId& AssetId, IInventoryProviderInterface* InventoryInterface) const;
	const TArray<FInventoryRecord>* GetRecords(FName ContainerId, const FInventoryStack* Stack) const;

	RENINVENTORY_API const FInventoryRecord* GetRecord(FName ContainerId, const FPrimaryAssetId& AssetId, int Index = 0) const;
	RENINVENTORY_API const FInventoryRecord* GetRecordById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId) const;

	RENINVENTORY_API void QueryItems(UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> Callback);

	UFUNCTION(BlueprintCallable)
	bool CreateContainer(FName ContainerId);

	UFUNCTION(BlueprintCallable)
	bool RemoveContainer(FName ContainerId);

protected:

	TWeakInterfacePtr<IInventoryProviderInterface> InventoryProvider;

	FInventoryStack* GetMutableInventoryStack(FName ContainerId, const FPrimaryAssetId& AssetId);
	FInventoryStack* GetMutableInventoryStack(FName InContainerId, const FPrimaryAssetId& AssetId, IInventoryProviderInterface* InventoryInterface);

	TArray<FInventoryRecord>* GetMutableRecords(FName ContainerId, const FPrimaryAssetId& AssetId);
	TArray<FInventoryRecord>* GetMutableRecords(FName ContainerId, const FPrimaryAssetId& AssetId, IInventoryProviderInterface* InventoryInterface);

	FInventoryRecord* GetMutableRecord(FName ContainerId, const FPrimaryAssetId& AssetId, int Index = 0);
	FInventoryRecord* GetMutableRecordById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId);

	virtual void HandleAddItem(const FPrimaryAssetId& AssetId, FInventoryRecord& Record);

	virtual void HandleItemSorting(TArray<FInventorySortEntry>& SortedItems, const FInventoryQueryRule& QueryRule) const;
	virtual void HandleGlossaryItems(UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> Callback) const;
	virtual void HandleInventoryItems(UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> Callback) const;

	virtual void HandleStorageLoaded();

private:

	TObjectPtr<UAssetManager> AssetManager;

	bool AddItemRecord(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity, FInventoryStack* Stack);

	bool RemoveItemRecord(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity, FInventoryStack* Stack);
	bool RemoveItemRecordById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId, int InQuantity, FInventoryStack* Stack);

	void AddItemRecord_Internal(FName ContainerId, const FPrimaryAssetId& AssetId, TArray<FInventoryRecord>& RecordList, int Quantity);
	void AddItemRecord_Internal(FName ContainerId, const FPrimaryAssetId& AssetId, FInventoryRecord& Record, int Quantity);

	bool RemoveItemRecord_Internal(FName ContainerId, const FPrimaryAssetId& AssetId, TArray<FInventoryRecord>* RecordList, int Quantity);
	bool RemoveItemRecord_Internal(FName ContainerId, const FPrimaryAssetId& AssetId, TArray<FInventoryRecord>* RecordList, FInventoryRecord* Record);
	void RemoveItemRecord_Internal(FName ContainerId, const FPrimaryAssetId& AssetId, TArray<FInventoryRecord>* RecordList, bool bPersistWhenEmpty, FInventoryRecord* Record, int Quantity);

protected:

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnInventoryItemChanged, FName /* ContainerId */, const FPrimaryAssetId& /* AssetId */, FName /* ItemId */);
	FOnInventoryItemChanged OnItemAdded;
	FOnInventoryItemChanged OnItemRemoved;
	FOnInventoryItemChanged OnItemUpdated;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnContainerChanged, FName /* ContainerId */);
	FOnContainerChanged OnContainerAdded;
	FOnContainerChanged OnContainerRemoved;

};

