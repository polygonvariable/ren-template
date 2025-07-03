// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenCore/Public/Macro/ValueMacro.h"
#include "RenCore/Public/Inventory/InventoryItemRarity.h"
#include "RenCore/Public/Inventory/InventoryItemType.h"

// Generated Headers
#include "InventoryAsset.generated.h"


/**
 *
 */
UCLASS()
class RENASSET_API UInventoryAsset : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Detail")
	FName AssetId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Detail")
	FText AssetName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Detail")
	FText AssetDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Detail")
	TSoftObjectPtr<UTexture2D> AssetIcon;


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inventory Item")
	TEnumAsByte<EInventoryItemType> ItemType;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inventory Item")
	TEnumAsByte<EInventoryItemRarity> ItemRarity;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inventory Item")
	bool bIsStackable = false;

};

