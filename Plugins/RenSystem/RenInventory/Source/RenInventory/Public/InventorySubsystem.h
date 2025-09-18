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
class UCraftableAsset;



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


	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool RemoveItem(FName ContainerId, FName RecordId, int Quantity);

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool RemoveItems(FName ContainerId, const TMap<FName, int>& ItemQuantities);


	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool DeleteItem(FName ContainerId, FName RecordId);

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API int DeleteItems(FName ContainerId, const TArray<FName>& RecordIds);


	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool ClearItems(FName ContainerId);

	UFUNCTION(BlueprintCallable)
	bool ReplaceItem(FName ContainerId, FName RecordId, FInventoryRecord Record);


	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool ContainsItem(FName ContainerId, FName RecordId, int Quantity) const;

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API bool ContainsItems(FName ContainerId, const TMap<FName, int>& Items) const;


	RENINVENTORY_API int CountItem(FName ContainerId, FName ItemId) const;

	RENINVENTORY_API bool UpdateItem(FName ContainerId, FName RecordId, TFunctionRef<bool(FInventoryRecord*)> InCallback);


	UFUNCTION(BlueprintCallable)
	bool CreateContainer(FName ContainerId);

	UFUNCTION(BlueprintCallable)
	bool RemoveContainer(FName ContainerId);



	RENINVENTORY_API const TMap<FName, FInventoryRecord>* GetRecords(FName ContainerId) const;
	RENINVENTORY_API const FInventoryRecord* GetItemRecord(FName ContainerId, FName RecordId) const;





	RENINVENTORY_API void QueryItems(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FPrimaryAssetId&, const FName&, const FInventoryRecord*)> InCallback) const;

protected:

	TWeakInterfacePtr<IInventoryProviderInterface> InventoryProvider;


	virtual void HandleGlossaryItems(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FPrimaryAssetId&, const FName&, const FInventoryRecord*)> InCallback) const;
	virtual void HandleInventoryItems(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FPrimaryAssetId&, const FName&, const FInventoryRecord*)> InCallback) const;
	virtual void HandleItemSorting(TArray<FInventorySortEntry>& SortedItems, const FInventoryQueryRule& QueryRule) const;


	const FInventoryRecord* AddItemRecord(const FPrimaryAssetId& InAssetId, int InQuantity, TMap<FName, FInventoryRecord>* InRecords, FName& OutRecordId);
	const FInventoryRecord* RemoveItemRecord(FName RecordId, int Quantity, TMap<FName, FInventoryRecord>* Records, bool bOutRemoved);

	virtual void HandleStorageLoaded();



	virtual TMap<FName, FInventoryRecord>* GetMutableRecords(FName ContainerId) const;
	FInventoryRecord* GetMutableItemRecord(FName ContainerId, FName RecordId) const;

private:

	UAssetManager* AssetManager;

protected:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:

	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnInventoryItemAdded, FName /* ContainerId */, FName /* RecordId */, const FInventoryRecord* /* ItemRecord */);
	FOnInventoryItemAdded OnItemAdded;

	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnInventoryItemRemoved, FName /* ContainerId */, FName /* RecordId */, const FInventoryRecord* /* ItemRecord */);
	FOnInventoryItemRemoved OnItemRemoved;

	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnInventoryItemUpdated, FName /* ContainerId */, FName /* RecordId */, const FInventoryRecord* /* ItemRecord */);
	FOnInventoryItemUpdated OnItemUpdated;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnContainerAdded, FName /* ContainerId */);
	FOnContainerAdded OnContainerAdded;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnContainerRemoved, FName /* ContainerId */);
	FOnContainerRemoved OnContainerRemoved;

};

