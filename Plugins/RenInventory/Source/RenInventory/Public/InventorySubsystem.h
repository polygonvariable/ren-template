// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "RenCore/Public/Inventory/InventoryItemType.h"
#include "RenCore/Public/Record/InventoryRecord.h"

#include "RenInventory/Public/InventoryDefinition.h"

// Generated Headers
#include "InventorySubsystem.generated.h"

// Forward Declarations
class UPrimaryAssetMap;
class UInventoryAsset;
class UGameMetadataSettings;
class IInventoryProviderInterface;



/**
 *
 */
UCLASS()
class RENINVENTORY_API UInventorySubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	bool AddItem(FName ContainerId, UInventoryAsset* ItemAsset, int Quantity = 1);

	UFUNCTION(BlueprintCallable)
	bool AddItems(FName ContainerId, const TMap<UInventoryAsset*, int>& ItemQuantities);


	UFUNCTION(BlueprintCallable)
	bool RemoveItem(FName ContainerId, FName ItemGuid, int Quantity);

	UFUNCTION(BlueprintCallable)
	bool RemoveItems(FName ContainerId, const TMap<FName, int>& ItemQuantities);

	UFUNCTION(BlueprintCallable)
	bool DeleteItem(FName ContainerId, FName ItemGuid);

	UFUNCTION(BlueprintCallable)
	bool DeleteItems(FName ContainerId, const TSet<FName>& ItemGuids);

	UFUNCTION(BlueprintCallable)
	bool ClearItems(FName ContainerId);

	UFUNCTION(BlueprintCallable)
	bool ReplaceItem(FName ContainerId, FName ItemGuid, FInventoryRecord Record);



	UFUNCTION(BlueprintCallable)
	bool ContainsItem(FName ContainerId, FName ItemGuid, int Quantity) const;

	UFUNCTION(BlueprintCallable)
	bool ContainsItems(FName ContainerId, const TMap<FName, int>& ItemQuantities) const;



	bool UpdateItem(FName ContainerId, FName ItemGuid, TFunctionRef<bool(FInventoryRecord*)> InCallback);


	UFUNCTION(BlueprintCallable)
	bool CreateContainer(FName ContainerId);

	UFUNCTION(BlueprintCallable)
	bool RemoveContainer(FName ContainerId);


	virtual TMap<FName, FInventoryRecord>* GetMutableRecords(FName ContainerId) const;
	virtual const TMap<FName, FInventoryRecord>* GetRecords(FName ContainerId) const;

	FInventoryRecord* GetMutableItemRecord(FName ContainerId, FName ItemGuid) const;
	const FInventoryRecord* GetItemRecord(FName ContainerId, FName ItemGuid) const;


	UFUNCTION(BlueprintCallable)
	UInventoryAsset* GetItemAsset(FName ItemId) const;

	template <class T>
	T* GetItemAsset(FName ItemId) const
	{
		return Cast<T>(GetItemAsset(ItemId));
	}

	virtual void QueryItems(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FName&, const FInventoryRecord*, UInventoryAsset*)> InCallback) const;


protected:

	TWeakInterfacePtr<IInventoryProviderInterface> InventoryInterface;

	UPROPERTY()
	TObjectPtr<UPrimaryAssetMap> InventoryAssetMap;


	virtual void HandleGlossaryItems(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FName&, const FInventoryRecord*, UInventoryAsset*)> InCallback) const;
	virtual void HandleInventoryItems(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FName&, const FInventoryRecord*, UInventoryAsset*)> InCallback) const;
	virtual void HandleItemSorting(TArray<FInventorySortEntry>& SortedItems, const FInventoryQueryRule& QueryRule) const;


	bool AddItemRecord_Internal(FName ItemId, EInventoryItemType ItemType, bool bIsStackable, int Quantity, TMap<FName, FInventoryRecord>* Records);
	bool RemoveItemRecord_Internal(FName ItemGuid, int Quantity, TMap<FName, FInventoryRecord>* Records);


	virtual void HandleStorageLoaded();

protected:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryItemAdded, const FInventoryRecord* /* ItemRecord */);
	FOnInventoryItemAdded OnItemAdded;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryItemRemoved, FInventoryRecord /* ItemRecord */);
	FOnInventoryItemRemoved OnItemRemoved;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryItemUpdated, const FInventoryRecord* /* ItemRecord */);
	FOnInventoryItemUpdated OnItemUpdated;


	DECLARE_MULTICAST_DELEGATE_OneParam(FOnContainerUpdated, FName);
	FOnContainerUpdated OnContainerUpdated;


	DECLARE_MULTICAST_DELEGATE_OneParam(FOnContainerAdded, FName /* ContainerId */);
	FOnContainerAdded OnContainerAdded;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnContainerRemoved, FName /* ContainerId */);
	FOnContainerRemoved OnContainerRemoved;

};

