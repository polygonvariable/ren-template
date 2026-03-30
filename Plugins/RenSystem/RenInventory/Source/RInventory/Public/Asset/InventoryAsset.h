// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Asset/InstancedDataAsset.h"
#include "Definition/InventoryItemRarity.h"
#include "Definition/InventoryItemType.h"
#include "Interface/IAssetComposition.h"

// Generated Headers
#include "InventoryAsset.generated.h"

// Module Macros
#define REN_API RINVENTORY_API

// Forward Declarations



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UInventoryAsset : public UInstancedDataAsset, public IAssetCompositionInterface
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable)
	EInventoryItemType Type = EInventoryItemType::Default;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable)
	EInventoryItemRarity Rarity = EInventoryItemRarity::Default;

	UPROPERTY(EditDefaultsOnly, AssetRegistrySearchable)
	bool bStackable = false;

	/**
	 * When true, the item will still be present in inventory even if the quantity is 0,
	 * this can be useful in things like bullets, as it can avoid constant adding &
	 * removal of key value pairs.
	 */
	UPROPERTY(EditDefaultsOnly, AssetRegistrySearchable)
	bool bPersistWhenEmpty = false;

	UPROPERTY(EditDefaultsOnly, AssetRegistrySearchable)
	bool bExpires = false;

	UPROPERTY(EditDefaultsOnly)
	FTimespan ExpirationDuration = FTimespan::Zero();


	// ~ IAssetCompositionInterface
	REN_API virtual const UAssetCollection* GetBreakdownAssets(const FGuid& InId) const override;
	REN_API virtual const UAssetCollection* GetRebuildAssets(const FGuid& InId) const override;
	// ~ End of IAssetCompositionInterface

	// ~ UPrimaryDataAsset
	REN_API virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UPrimaryDataAsset

public:

	REN_API static FPrimaryAssetType GetPrimaryAssetType();

};



// Module Macros
#undef REN_API

