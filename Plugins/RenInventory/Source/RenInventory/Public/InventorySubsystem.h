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
UENUM(BlueprintType)
enum class EInventoryQuerySource : uint8
{
	Inventory UMETA(DisplayName = "Inventory"),
	Glossary UMETA(DisplayName = "Glossary"),
};

/**
 *
 */
UENUM(BlueprintType)
enum class ESortDirection : uint8
{
	Ascending UMETA(DisplayName = "Ascending"),
	Descending UMETA(DisplayName = "Descending"),
};

/**
 *
 */
UENUM(BlueprintType)
enum class EInventorySortType : uint8
{
	None UMETA(DisplayName = "None"),
	Alphabetical UMETA(DisplayName = "Alphabetical"),
	Quantity UMETA(DisplayName = "Quantity"),
	Rank UMETA(DisplayName = "Rank"),
	Level UMETA(DisplayName = "Level"),
};


/**
 *
 */
USTRUCT(BlueprintType)
struct FInventoryQueryRule
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInventoryQuerySource QuerySource = EInventoryQuerySource::Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESortDirection SortDirection = ESortDirection::Ascending;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInventorySortType SortType = EInventorySortType::Alphabetical;

};

struct FInventorySortEntry
{

	FName Guid = NAME_None;
	const FInventoryRecord* Record = nullptr;
	UInventoryAsset* Asset = nullptr;

	FInventorySortEntry(FName InGuid, const FInventoryRecord* InRecord, UInventoryAsset* InAsset) : Guid(InGuid), Record(InRecord), Asset(InAsset) {}

};

/**
 *
 */
UCLASS()
class RENINVENTORY_API UInventorySubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	bool AddItem(UInventoryAsset* ItemAsset, int Quantity = 1);

	UFUNCTION(BlueprintCallable)
	bool AddItems(const TMap<UInventoryAsset*, int>& ItemQuantities);

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(FName ItemGuid, int Quantity);

	UFUNCTION(BlueprintCallable)
	bool RemoveItems(const TMap<FName, int>& ItemQuantities);

	UFUNCTION(BlueprintCallable)
	bool DestroyItem(FName ItemGuid);

	UFUNCTION(BlueprintCallable)
	bool DestroyItems(const TSet<FName>& ItemGuids);

	UFUNCTION(BlueprintCallable)
	bool ClearItems();


	UFUNCTION(BlueprintCallable)
	bool UpdateItem(FName ItemGuid, FInventoryRecord Record);

	UFUNCTION(BlueprintCallable)
	bool ContainsItem(FName ItemGuid);



	UFUNCTION(BlueprintCallable)
	FInventoryRecord GetItemRecord(FName ItemGuid) const;

	UFUNCTION(BlueprintCallable)
	TMap<FName, FInventoryRecord> GetAllItemRecords();

	UFUNCTION(BlueprintCallable)
	UInventoryAsset* GetItemAsset(const FName& ItemId) const;


	virtual void QueryItems(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FName&, const FInventoryRecord*, UInventoryAsset*)> InCallback) const;


protected:

	TWeakInterfacePtr<IInventoryProviderInterface> InventoryInterface;

	UPROPERTY()
	TObjectPtr<UPrimaryAssetMap> InventoryAssetMap;


	virtual void HandleGlossaryItems(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FName&, const FInventoryRecord*, UInventoryAsset*)> InCallback) const;
	virtual void HandleInventoryItems(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FName&, const FInventoryRecord*, UInventoryAsset*)> InCallback) const;
	virtual void HandleItemSorting(TArray<FInventorySortEntry>& SortedItems, const FInventoryQueryRule& QueryRule) const;


	void HandleStorageLoaded();


	bool AddItemRecord_Internal(FName ItemId, EInventoryItemType ItemType, bool bIsStackable, int Quantity, TMap<FName, FInventoryRecord>& Records);
	bool RemoveItemRecord_Internal(FName ItemGuid, int Quantity, TMap<FName, FInventoryRecord>& Records);

protected:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:

	DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);
	FOnInventoryUpdated OnInventoryUpdated;

};

