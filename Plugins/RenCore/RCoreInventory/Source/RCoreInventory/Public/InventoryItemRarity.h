// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Generated Headers
#include "InventoryItemRarity.generated.h"



/**
 *
 */
UENUM(BlueprintType)
enum class EInventoryItemRarity : uint8
{

    // DO NOT REMOVE
    Default UMETA(DisplayName = "Default"),

    // Add new types below this line only
    Common UMETA(DisplayName = "Common"),
    Rare UMETA(DisplayName = "Rare"),
    Epic UMETA(DisplayName = "Epic"),

};

