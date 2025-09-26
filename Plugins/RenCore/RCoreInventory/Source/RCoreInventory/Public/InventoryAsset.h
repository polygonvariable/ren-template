// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

// Project Headers
#include "RCoreExchange/Public/ExchangeInterface.h"

#include "RCoreInventory/Public/InventoryItemRarity.h"
#include "RCoreInventory/Public/InventoryItemType.h"

// Generated Headers
#include "InventoryAsset.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS(MinimalAPI)
class UInventoryAsset : public UPrimaryDataAsset, public IExchangeInterface
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

	// ~ UPrimaryDataAsset
	RCOREINVENTORY_API virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UPrimaryDataAsset

	// ~ IExchangeInterface
	RCOREINVENTORY_API virtual float GetExchangedNumber(FInstancedStruct& Context) const override;
	RCOREINVENTORY_API virtual FName GetExchangedText(FInstancedStruct& Context) const override;
	// ~ End of IExchangeInterface

};

