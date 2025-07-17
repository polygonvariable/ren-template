// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "RenCore/Public/Inventory/InventoryItemType.h"
#include "RenCore/Public/Record/InventoryRecord.h"

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
	bool AddItem(UInventoryAsset* InventoryAsset, int Quantity = 1);

	UFUNCTION(BlueprintCallable)
	bool AddItems(const TMap<UInventoryAsset*, int>& Items);

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(FName ItemGuid, int Quantity = 1);

	UFUNCTION(BlueprintCallable)
	bool RemoveItems(const TMap<FName, int>& Items);

	UFUNCTION(BlueprintCallable)
	bool UpdateItem(FName ItemGuid, FInventoryRecord ItemRecord);

	UFUNCTION(BlueprintCallable)
	bool ContainsItem(FName ItemGuid);



	UFUNCTION(BlueprintCallable)
	FInventoryRecord GetItemRecord(FName ItemGuid);

	UFUNCTION(BlueprintCallable)
	TMap<FName, FInventoryRecord> GetAllItemRecords();

	UFUNCTION(BlueprintCallable)
	UInventoryAsset* GetItemAsset(const FName& ItemId) const;


	void ForEachItem(TFunctionRef<void(const FName&, const FInventoryRecord&, UInventoryAsset*)> InCallback) const;


protected:

	TWeakInterfacePtr<IInventoryProviderInterface> InventoryInterface;

	UPROPERTY()
	TObjectPtr<UPrimaryAssetMap> InventoryAssetMap;


	void HandleStorageLoaded();


	bool AddItemRecord_Internal(FName ItemId, EInventoryItemType ItemType, bool bIsStackable, int Quantity, TMap<FName, FInventoryRecord>& Records);
	bool RemoveItemRecord_Internal(FName ItemId, int Quantity, TMap<FName, FInventoryRecord>& Records);

protected:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:

	DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);
	FOnInventoryUpdated OnInventoryUpdated;

};

