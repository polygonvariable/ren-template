// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenCore/Public/Record/InventoryRecord.h"

// Generated Headers
#include "InventoryDefinition.generated.h"

// Forward Declarations
class UInventoryAsset;



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

	FName Guid = NAME_None;
	const FInventoryRecord* Record = nullptr;
	UInventoryAsset* Asset = nullptr;

	FInventorySortEntry(FName InGuid, const FInventoryRecord* InRecord, UInventoryAsset* InAsset) : Guid(InGuid), Record(InRecord), Asset(InAsset) {}

};

