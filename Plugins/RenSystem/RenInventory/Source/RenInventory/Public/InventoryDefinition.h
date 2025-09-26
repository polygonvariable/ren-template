// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RCoreInventory/Public/InventoryItemRarity.h"
#include "RCoreInventory/Public/InventoryItemType.h"

// Generated Headers
#include "InventoryDefinition.generated.h"

// Forward Declarations
class UInventoryAsset;

struct FInventoryRecord;



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
	FName ContainerId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInventoryQuerySource QuerySource = EInventoryQuerySource::Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESortDirection SortDirection = ESortDirection::Ascending;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInventorySortType SortType = EInventorySortType::Alphabetical;

};


/**
 *
 */
struct FInventorySortEntry
{

	FPrimaryAssetId AssetId = FPrimaryAssetId();
	FText ItemName = FText::GetEmpty();
	int ItemQuantity = 0;
	const FInventoryRecord* Record = nullptr;

	FInventorySortEntry(FPrimaryAssetId InAssetId, FText InItemName, int InItemQuantity) :
		AssetId(InAssetId),
		ItemName(InItemName),
		ItemQuantity(InItemQuantity)
	{}

	FInventorySortEntry(FPrimaryAssetId InAssetId, FText InItemName, int InItemQuantity, const FInventoryRecord* InRecord) :
		AssetId(InAssetId),
		ItemName(InItemName),
		ItemQuantity(InItemQuantity),
		Record(InRecord)
	{}

};


/**
 *
 */
USTRUCT()
struct FInventoryPayload
{

	GENERATED_BODY()

};

/**
 *
 */
USTRUCT()
struct FInventoryPayloadQuantity : public FInventoryPayload
{

	GENERATED_BODY()

public:

	int Quantity = 0;

};



