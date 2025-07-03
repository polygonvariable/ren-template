// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName AssetId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText AssetName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText AssetDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> AssetIcon;


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TEnumAsByte<EInventoryItemType> ItemType;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TEnumAsByte<EInventoryItemRarity> ItemRarity;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bIsStackable = false;

};

