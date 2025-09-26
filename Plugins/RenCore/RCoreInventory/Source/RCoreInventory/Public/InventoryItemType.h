// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Generated Headers
#include "InventoryItemType.generated.h"



/**
 *
 */
UENUM(BlueprintType)
enum class EInventoryItemType : uint8
{

    // DO NOT REMOVE
    Default UMETA(DisplayName = "Default"),

    // Add new types below this line only
    Food UMETA(DisplayName = "Food"),
    Weapon UMETA(DisplayName = "Weapon"),
    Skill UMETA(DisplayName = "Skill"),
    Currency UMETA(DisplayName = "Currency"),
    Photo UMETA(DisplayName = "Photo")

};

