// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

// Project Headers
#include "RenCoreInventory/Public/InventoryItemRarity.h"
#include "RenCoreInventory/Public/InventoryItemType.h"

#include "RenAsset/Public/Inventory/Type/InventoryAssetQuantity.h"

// Generated Headers
#include "AssetMetadata.generated.h"

// Forward Declarations
class UEnhanceAsset;



/**
 *
 */
UENUM(BlueprintType)
enum EAssetMetadataType : uint8
{
	Enhanceable UMETA(DisplayName = "Enhanceable"),
	Enhancement UMETA(DisplayName = "Enhancement")
};



/**
 *
 */
UCLASS()
class RENASSET_API UAssetMetadata : public UDataAsset
{

	GENERATED_BODY()

};


/**
 *
 */
UCLASS()
class RENASSET_API UExchangeMetadata : public UAssetMetadata
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Exchange")
	int XpInterval = 5000;

};


