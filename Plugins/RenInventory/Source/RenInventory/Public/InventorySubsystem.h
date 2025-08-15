// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "RenCoreInventory/Public/InventoryFilterRule.h"
#include "RenCoreInventory/Public/InventoryItemType.h"
#include "RenCoreInventory/Public/InventoryManagerInterface.h"

#include "RenInventory/Public/InventoryDefinition.h"

// Generated Headers
#include "InventorySubsystem.generated.h"

// Forward Declarations
class UPrimaryAssetMap;
class UInventoryAsset;
class UGameMetadataSettings;
class IInventoryProviderInterface;

struct FInventoryRecord;
class UCraftableAsset;



/**
 *
 */
UCLASS(MinimalAPI)
class UInventorySubsystem : public UGameInstanceSubsystem, public IInventoryManagerInterface
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	bool BP_AddItem(FName ContainerId, UInventoryAsset* ItemAsset, int Quantity);

	UFUNCTION(BlueprintCallable)
	bool BP_AddItems(FName ContainerId, const TMap<UInventoryAsset*, int>& ItemQuantities);


	virtual bool AddItem(FName ContainerId, UInventoryAsset* ItemAsset, int Quantity) override;
	virtual bool AddItems(FName ContainerId, const TMap<UInventoryAsset*, int>& ItemQuantities) override;


	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool RemoveItem(FName ContainerId, FName ItemGuid, int Quantity);

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool RemoveItems(FName ContainerId, const TMap<FName, int>& ItemQuantities);


	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool DeleteItem(FName ContainerId, FName ItemGuid);

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool DeleteItems(FName ContainerId, const TSet<FName>& ItemGuids);


	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool ClearItems(FName ContainerId);

	UFUNCTION(BlueprintCallable)
	bool ReplaceItem(FName ContainerId, FName ItemGuid, FInventoryRecord Record);


	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool ContainsItem(FName ContainerId, FName ItemGuid, int Quantity) const;

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool ContainsItems(FName ContainerId, const TMap<FName, int>& ItemQuantities) const;


	RENINVENTORY_API int CountItem(FName ContainerId, FName ItemId) const;

	RENINVENTORY_API bool UpdateItem(FName ContainerId, FName ItemGuid, TFunctionRef<bool(FInventoryRecord*)> InCallback);


	UFUNCTION(BlueprintCallable)
	bool CreateContainer(FName ContainerId);

	UFUNCTION(BlueprintCallable)
	bool RemoveContainer(FName ContainerId);


	RENINVENTORY_API const TMap<FName, FInventoryRecord>* GetRecords(FName ContainerId) const;

	RENINVENTORY_API const FInventoryRecord* GetItemRecord(FName ContainerId, FName ItemGuid) const;

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API UInventoryAsset* GetItemAsset(FName ItemId) const;

	template <class T>
	T* GetItemAsset(FName ItemId) const
	{
		return Cast<T>(GetItemAsset(ItemId));
	}

	RENINVENTORY_API void QueryItems(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FName&, const FInventoryRecord*, UInventoryAsset*)> InCallback) const;

protected:

	TWeakInterfacePtr<IInventoryProviderInterface> InventoryInterface;

	UPROPERTY()
	TObjectPtr<UPrimaryAssetMap> InventoryAssetMap;


	virtual void HandleGlossaryItems(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FName&, const FInventoryRecord*, UInventoryAsset*)> InCallback) const;
	virtual void HandleInventoryItems(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FName&, const FInventoryRecord*, UInventoryAsset*)> InCallback) const;
	virtual void HandleItemSorting(TArray<FInventorySortEntry>& SortedItems, const FInventoryQueryRule& QueryRule) const;


	bool AddItemRecord_Internal(FName ContainerId, FName ItemId, EInventoryItemType ItemType, bool bIsStackable, int Quantity, TMap<FName, FInventoryRecord>* Records);
	bool RemoveItemRecord_Internal(FName ItemGuid, int Quantity, TMap<FName, FInventoryRecord>* Records);


	virtual void HandleStorageLoaded();

	virtual TMap<FName, FInventoryRecord>* GetMutableRecords(FName ContainerId) const;

	FInventoryRecord* GetMutableItemRecord(FName ContainerId, FName ItemGuid) const;

protected:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:

	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnInventoryItemAdded, FName /* ContainerId */, FName /* ItemGuid */, const FInventoryRecord* /* ItemRecord */);
	FOnInventoryItemAdded OnItemAdded;

	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnInventoryItemRemoved, FName /* ContainerId */, FName /* ItemGuid */, FInventoryRecord /* ItemRecord */);
	FOnInventoryItemRemoved OnItemRemoved;

	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnInventoryItemUpdated, FName /* ContainerId */, FName /* ItemGuid */, const FInventoryRecord* /* ItemRecord */);
	FOnInventoryItemUpdated OnItemUpdated;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnContainerAdded, FName /* ContainerId */);
	FOnContainerAdded OnContainerAdded;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnContainerRemoved, FName /* ContainerId */);
	FOnContainerRemoved OnContainerRemoved;

};

