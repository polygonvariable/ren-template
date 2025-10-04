// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

#include "Engine/DataAsset.h"
#include "InstancedStruct.h"

// Project Headers
#include "RCoreExchange/Public/ExchangeProviderInterface.h"

#include "RCoreInventory/Public/InventoryItemRarity.h"
#include "RCoreInventory/Public/InventoryItemType.h"

// Generated Headers
#include "InventoryAsset.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UInventoryAsset : public UPrimaryDataAsset, public IExchangeProviderInterface
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FName ItemId = NAME_None;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, AssetRegistrySearchable)
	FText DisplayName = FText::GetEmpty();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText Description = FText::GetEmpty();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, AssetRegistrySearchable)
	EInventoryItemType Type = EInventoryItemType::Default;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, AssetRegistrySearchable)
	EInventoryItemRarity Rarity = EInventoryItemRarity::Default;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, AssetRegistrySearchable)
	bool bStackable = false;

	/*
	 * When true, the item will still be present in inventory even if the quantity is 0,
	 * this can be useful in things like bullets, as it can avoid constant adding &
	 * removal of key value pairs.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, AssetRegistrySearchable)
	bool bPersistWhenEmpty = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, AssetRegistrySearchable)
	bool bExpires = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FTimespan ExpirationDuration = FTimespan::Zero();


	RCOREINVENTORY_API virtual FInstancedStruct GetMetadata();

public:

	// ~ UPrimaryDataAsset
	RCOREINVENTORY_API virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UPrimaryDataAsset

	// ~ IExchangeProviderInterface
	RCOREINVENTORY_API virtual float GetExchangedNumber(FInstancedStruct& Context) const override;
	RCOREINVENTORY_API virtual FName GetExchangedText(FInstancedStruct& Context) const override;
	// ~ End of IExchangeProviderInterface

};

