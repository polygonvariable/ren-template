// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Generated Headers
#include "InventoryItemType.generated.h"

// TODO:
// Replace with gameplay tags
// Inventory.Type.*

/**
 *
 */
UENUM(BlueprintType, DisplayName = "Inventory Item Type")
enum EInventoryItemType : uint8
{
    Food UMETA(DisplayName = "Food"),
    Weapon UMETA(DisplayName = "Weapon"),
    Skill UMETA(DisplayName = "Skill"),
    Currency UMETA(DisplayName = "Currency"),
    Photo UMETA(DisplayName = "Photo")
};

