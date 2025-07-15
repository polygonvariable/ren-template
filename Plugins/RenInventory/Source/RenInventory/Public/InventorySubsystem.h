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
	bool AddRecord(UInventoryAsset* InventoryAsset, int Quantity = 1);

	UFUNCTION(BlueprintCallable)
	bool AddRecords(const TMap<UInventoryAsset*, int32>& Items);

	UFUNCTION(BlueprintCallable)
	bool RemoveRecord(FName ItemGuid, int Quantity = 1);

	UFUNCTION(BlueprintCallable)
	bool RemoveRecords(const TMap<FName, int32>& Items);

	UFUNCTION(BlueprintCallable)
	bool UpdateRecord(FName ItemGuid, FInventoryRecord InventoryRecord);

	UFUNCTION(BlueprintCallable)
	bool HasRecord(FName ItemGuid);



	UFUNCTION(BlueprintCallable)
	bool GetRecord(FName ItemGuid, FInventoryRecord& OutInventoryRecord);

	UFUNCTION(BlueprintCallable)
	TMap<FName, FInventoryRecord> GetRecords();

	UFUNCTION(BlueprintCallable)
	UInventoryAsset* GetRecordAsset(FName InventoryAssetId);

protected:

	TWeakInterfacePtr<IInventoryProviderInterface> InventoryInterface;

	UPROPERTY()
	TObjectPtr<UPrimaryAssetMap> InventoryAssetMap;


	void HandleStorageLoaded();


	bool InternalAddRecord(FName ItemId, EInventoryItemType ItemType, bool bIsStackable, int Quantity, TMap<FName, FInventoryRecord>& Records);
	bool InternalRemoveRecord(FName ItemId, int Quantity, TMap<FName, FInventoryRecord>& Records);

protected:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:

	DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);
	FOnInventoryUpdated OnInventoryUpdated;

};

