// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Generated Headers
#include "InventoryItemRarity.generated.h"

// TODO:
// Replace with gameplay tags
// Inventory.Rarity.*

/**
 *
 */
UENUM(BlueprintType, DisplayName = "Inventory Item Rarity")
enum EInventoryItemRarity : uint8
{
    Common UMETA(DisplayName = "Common"),
    Rare UMETA(DisplayName = "Rare"),
    Epic UMETA(DisplayName = "Epic"),
};

