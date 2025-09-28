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
class UCraftableAsset;
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

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool AddItems(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items);


	/*
	 * Removes the first item in the item stack.
	 * Useful for stackable items.
	 */
	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool RemoveItem(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity);

	/*
	 * Removes the first item in the item stack.
	 * Useful for stackable items.
	 */
	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool RemoveItems(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items);

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool RemoveItemById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId, int Quantity);



	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool ContainsItem(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity) const;

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool ContainsItems(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items) const;

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool ContainsItemById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId, int Quantity) const;


	RENINVENTORY_API bool UpdateItem(FName ContainerId, const FPrimaryAssetId& AssetId, TFunctionRef<bool(FInventoryRecord*)> InCallback);
	RENINVENTORY_API bool UpdateItemById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId, TFunctionRef<bool(FInventoryRecord*)> InCallback);


	RENINVENTORY_API int GetItemCount(FName ContainerId, const FPrimaryAssetId& AssetId) const;
	RENINVENTORY_API int GetItemCount(FName ContainerId, const FPrimaryAssetId& AssetId, IInventoryProviderInterface* InventoryInterface) const;

	RENINVENTORY_API int GetItemQuantity(FName ContainerId, const FPrimaryAssetId& AssetId, int Index = 0) const;
	RENINVENTORY_API int GetItemQuantity(FName ContainerId, const FPrimaryAssetId& AssetId, IInventoryProviderInterface* InventoryInterface, int Index = 0) const;


	RENINVENTORY_API const TArray<FInventoryRecord>* GetItemStack(FName ContainerId, FName ItemId) const;
	RENINVENTORY_API const TArray<FInventoryRecord>* GetItemStack(FName ContainerId, FName ItemId, IInventoryProviderInterface* InventoryInterface) const;

	RENINVENTORY_API const FInventoryRecord* GetRecordByIndex(FName ContainerId, const FPrimaryAssetId& AssetId, int Index = 0) const;
	RENINVENTORY_API const FInventoryRecord* GetRecordById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId) const;

	RENINVENTORY_API void QueryItems(UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> InCallback);


	UFUNCTION(BlueprintCallable)
	bool CreateContainer(FName ContainerId);

	UFUNCTION(BlueprintCallable)
	bool RemoveContainer(FName ContainerId);

protected:

	TWeakInterfacePtr<IInventoryProviderInterface> InventoryProvider;

	TArray<FInventoryRecord>* GetMutableItemStack(FName ContainerId, FName ItemId);
	TArray<FInventoryRecord>* GetMutableItemStack(FName ContainerId, FName ItemId, IInventoryProviderInterface* InventoryInterface);

	FInventoryRecord* GetMutableRecordByIndex(FName ContainerId, const FPrimaryAssetId& AssetId, int Index = 0);
	FInventoryRecord* GetMutableRecordById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId);
	
	virtual void HandleItemSorting(TArray<FInventorySortEntry>& SortedItems, const FInventoryQueryRule& QueryRule) const;
	virtual void HandleGlossaryItems(UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> InCallback) const;
	virtual void HandleInventoryItems(UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> InCallback) const;

	virtual void HandleStorageLoaded();

	virtual void PreAddItem(FInventoryRecord& Record);

private:

	UAssetManager* AssetManager;

	const FInventoryRecord* AddItemRecord(const FPrimaryAssetId& InAssetId, int InQuantity, TArray<FInventoryRecord>* InItemStack, FName& OutRecordId);
	const FInventoryRecord* RemoveItemRecord(const FPrimaryAssetId& InAssetId, int InQuantity, TArray<FInventoryRecord>* InItemStack, bool& bOutRemoved);
	const FInventoryRecord* RemoveItemRecordById(const FPrimaryAssetId& InAssetId, FName InRecordId, int InQuantity, TArray<FInventoryRecord>* InItemStack, bool& bOutRemoved);

	const FInventoryRecord* RemoveItemRecord_Internal(const FPrimaryAssetId& InAssetId, FInventoryRecord* InRecord, int InQuantity, TArray<FInventoryRecord>* InItemStack, bool& bOutRemoved);

protected:

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnInventoryItemChanged, FName /* ContainerId */, const FPrimaryAssetId& /* AssetId */, const FInventoryRecord* /* ItemRecord */);
	FOnInventoryItemChanged OnItemAdded;
	FOnInventoryItemChanged OnItemRemoved;
	FOnInventoryItemChanged OnItemUpdated;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnContainerChanged, FName /* ContainerId */);
	FOnContainerChanged OnContainerAdded;
	FOnContainerChanged OnContainerRemoved;

};

