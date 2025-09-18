// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

// Project Headers
#include "RCoreInventory/Public/InventoryItemRarity.h"
#include "RCoreInventory/Public/InventoryItemType.h"

// Generated Headers
#include "InventoryAsset.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS(MinimalAPI)
class UInventoryAsset : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FName ItemId = NAME_None;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, AssetRegistrySearchable)
	FText ItemName = FText::GetEmpty();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText ItemDescription = FText::GetEmpty();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> ItemIcon = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, AssetRegistrySearchable)
	EInventoryItemType ItemType = EInventoryItemType::Default;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, AssetRegistrySearchable)
	EInventoryItemRarity ItemRarity = EInventoryItemRarity::Default;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, AssetRegistrySearchable)
	bool bIsStackable = false;

	/*
	 * When true, the item will still be present in inventory even if the quantity is 0,
	 * this can be useful in things like bullets, as it can avoid constant adding &
	 * removal of key value pairs.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, AssetRegistrySearchable)
	bool bAllowEmptyData = false;

public:

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("Inventory"), GetFName());
	}

};

