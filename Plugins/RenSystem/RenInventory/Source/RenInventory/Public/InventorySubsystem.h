// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "RCoreInventory/Public/InventoryFilterRule.h"
#include "RCoreInventory/Public/InventoryItemType.h"
#include "RCoreInventory/Public/InventoryManagerInterface.h"

#include "RenInventory/Public/InventoryDefinition.h"

// Generated Headers
#include "InventorySubsystem.generated.h"

// Forward Declarations
class UAssetManager;

class UPrimaryAssetMap;
class UInventoryAsset;
class UGameMetadataSettings;
class IInventoryProviderInterface;

struct FInventoryRecord;
struct FInventoryStack;

class UCraftableAsset;



/**
 *
 */
UCLASS(MinimalAPI)
class UInventorySubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:/*

	UE_DEPRECATED(5.4, "Marked to be removed")
	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool AddItem(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity);

	UE_DEPRECATED(5.4, "Marked to be removed")
	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool AddItems(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items);*/






	RENINVENTORY_API const TArray<FInventoryRecord>* GetItemStack(FName ContainerId, FName ItemId) const;
	RENINVENTORY_API const TArray<FInventoryRecord>* GetItemStack(FName ContainerId, FName ItemId, IInventoryProviderInterface* InventoryInterface) const;

	RENINVENTORY_API TArray<FInventoryRecord>* GetMutableItemStack(FName ContainerId, FName ItemId);
	RENINVENTORY_API TArray<FInventoryRecord>* GetMutableItemStack(FName ContainerId, FName ItemId, IInventoryProviderInterface* InventoryInterface);

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool AddItem2(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity);
	RENINVENTORY_API bool AddItems2(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items);

	RENINVENTORY_API bool RemoveItem2(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity); // Removes the first item in the item stack. Useful for stackable items
	RENINVENTORY_API bool RemoveItems2(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items);
	RENINVENTORY_API bool RemoveItemById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId, int Quantity);

	RENINVENTORY_API bool ContainsItem2(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity) const;
	RENINVENTORY_API bool ContainsItems2(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items) const;

	RENINVENTORY_API bool ContainsItemById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId, int Quantity) const;

	RENINVENTORY_API bool UpdateItem2(FName ContainerId, const FPrimaryAssetId& AssetId, TFunctionRef<bool(FInventoryRecord*)> InCallback);
	RENINVENTORY_API bool UpdateItemById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId, TFunctionRef<bool(FInventoryRecord*)> InCallback);

	RENINVENTORY_API int GetItemCount(FName ContainerId, const FPrimaryAssetId& AssetId) const;
	RENINVENTORY_API int GetItemCount(FName ContainerId, const FPrimaryAssetId& AssetId, IInventoryProviderInterface* InventoryInterface) const;

	RENINVENTORY_API int GetItemQuantity(FName ContainerId, const FPrimaryAssetId& AssetId, int Index = 0) const;
	RENINVENTORY_API int GetItemQuantity(FName ContainerId, const FPrimaryAssetId& AssetId, IInventoryProviderInterface* InventoryInterface, int Index = 0) const;

	RENINVENTORY_API const FInventoryRecord* GetRecordByIndex(FName ContainerId, const FPrimaryAssetId& AssetId, int Index = 0) const;
	RENINVENTORY_API const FInventoryRecord* GetRecordById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId) const;

	RENINVENTORY_API FInventoryRecord* GetMutableRecordByIndex(FName ContainerId, const FPrimaryAssetId& AssetId, int Index = 0);
	RENINVENTORY_API FInventoryRecord* GetMutableRecordById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId);

	RENINVENTORY_API virtual void HandleGlossaryItems2(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> InCallback) const;
	RENINVENTORY_API virtual void HandleInventoryItems2(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> InCallback) const;




	/*
	UE_DEPRECATED(5.4, "Marked to be removed")
	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool RemoveItem(FName ContainerId, FName RecordId, int Quantity);

	UE_DEPRECATED(5.4, "Marked to be removed")
	RENINVENTORY_API bool RemoveItems(FName ContainerId, const TMap<FName, int>& ItemQuantities);

	UE_DEPRECATED(5.4, "Marked to be removed")
	RENINVENTORY_API bool RemoveItems(FName ContainerId, const TMap<FPrimaryAssetId, int>& ItemQuantities);


	UE_DEPRECATED(5.4, "Marked to be removed")
	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool DeleteItem(FName ContainerId, FName RecordId);

	UE_DEPRECATED(5.4, "Marked to be removed")
	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API int DeleteItems(FName ContainerId, const TArray<FName>& RecordIds);

	UE_DEPRECATED(5.4, "Marked to be removed")
	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool ClearItems(FName ContainerId);

	UE_DEPRECATED(5.4, "Marked to be removed")
	UFUNCTION(BlueprintCallable)
	bool ReplaceItem(FName ContainerId, FName RecordId, FInventoryRecord Record);

	UE_DEPRECATED(5.4, "Marked to be removed")
	RENINVENTORY_API bool ContainsItem(FName ContainerId, FName RecordId, int Quantity) const;

	UE_DEPRECATED(5.4, "Marked to be removed")
	RENINVENTORY_API bool ContainsItem(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity) const;

	UE_DEPRECATED(5.4, "Marked to be removed")
	RENINVENTORY_API bool ContainsItems(FName ContainerId, const TMap<FName, int>& Items) const;

	UE_DEPRECATED(5.4, "Marked to be removed")
	RENINVENTORY_API bool ContainsItems(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items) const;

	UE_DEPRECATED(5.4, "Marked to be removed")
	RENINVENTORY_API int CountItem(FName ContainerId, FName ItemId) const;

	UE_DEPRECATED(5.4, "Marked to be removed")
	RENINVENTORY_API bool UpdateItem(FName ContainerId, FName RecordId, TFunctionRef<bool(FInventoryRecord*)> InCallback);
	*/


	UFUNCTION(BlueprintCallable)
	bool CreateContainer(FName ContainerId);

	UFUNCTION(BlueprintCallable)
	bool RemoveContainer(FName ContainerId);



	/*
	UE_DEPRECATED(5.4, "Marked to be removed")
	RENINVENTORY_API const TMap<FName, FInventoryRecord>* GetRecords(FName ContainerId) const;

	UE_DEPRECATED(5.4, "Marked to be removed")
	RENINVENTORY_API const FInventoryRecord* GetItemRecord(FName ContainerId, FName RecordId) const;

	UE_DEPRECATED(5.4, "Marked to be removed")
	RENINVENTORY_API void QueryItems(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FPrimaryAssetId&, const FName&, const FInventoryRecord*)> InCallback) const;
	*/
protected:

	TWeakInterfacePtr<IInventoryProviderInterface> InventoryProvider;

	/*
	UE_DEPRECATED(5.4, "Marked to be removed")
	virtual void HandleGlossaryItems(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FPrimaryAssetId&, const FName&, const FInventoryRecord*)> InCallback) const;

	UE_DEPRECATED(5.4, "Marked to be removed")
	virtual void HandleInventoryItems(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FPrimaryAssetId&, const FName&, const FInventoryRecord*)> InCallback) const;
	
	UE_DEPRECATED(5.4, "Marked to be removed")
	const FInventoryRecord* AddItemRecord(const FPrimaryAssetId& InAssetId, int InQuantity, TMap<FName, FInventoryRecord>* InRecords, FName& OutRecordId);

	UE_DEPRECATED(5.4, "Marked to be removed")
	const FInventoryRecord* RemoveItemRecord(FName RecordId, int Quantity, TMap<FName, FInventoryRecord>* Records, bool bOutRemoved);

	UE_DEPRECATED(5.4, "Marked to be removed")
	virtual TMap<FName, FInventoryRecord>* GetMutableRecords(FName ContainerId) const;

	UE_DEPRECATED(5.4, "Marked to be removed")
	FInventoryRecord* GetMutableItemRecord(FName ContainerId, FName RecordId) const;
	*/

	virtual void HandleItemSorting(TArray<FInventorySortEntry>& SortedItems, const FInventoryQueryRule& QueryRule) const;

	virtual void PreAddItem(FInventoryRecord& Record);
	const FInventoryRecord* AddItemRecord(const FPrimaryAssetId& InAssetId, int InQuantity, TArray<FInventoryRecord>* InItemStack, FName& OutRecordId);
	const FInventoryRecord* RemoveItemRecord(const FPrimaryAssetId& InAssetId, int InQuantity, TArray<FInventoryRecord>* InItemStack, bool bOutRemoved);
	const FInventoryRecord* RemoveItemRecordById(const FPrimaryAssetId& InAssetId, FName InRecordId, int InQuantity, TArray<FInventoryRecord>* InItemStack, bool bOutRemoved);

	virtual void HandleStorageLoaded();

private:

	UAssetManager* AssetManager;

	const FInventoryRecord* RemoveItemRecord_Internal(const FPrimaryAssetId& InAssetId, FInventoryRecord* InRecord, int InQuantity, TArray<FInventoryRecord>* InItemStack, bool bOutRemoved);

protected:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:

	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnInventoryItemChanged, FName /* ContainerId */, const FPrimaryAssetId& /* AssetId */, const FInventoryRecord* /* ItemRecord */);
	FOnInventoryItemChanged OnItemAdded;
	FOnInventoryItemChanged OnItemRemoved;
	FOnInventoryItemChanged OnItemUpdated;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnContainerChanged, FName /* ContainerId */);
	FOnContainerChanged OnContainerAdded;
	FOnContainerChanged OnContainerRemoved;

};

