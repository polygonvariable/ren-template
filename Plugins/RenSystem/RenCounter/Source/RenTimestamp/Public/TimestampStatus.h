// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"



/**
 *
 */
UENUM(BlueprintType)
enum class ETimestampStatus : uint8
{
	Changed UMETA(DisplayName = "Changed"),
	Unchanged UMETA(DisplayName = "Unchanged"),
	Invalid UMETA(DisplayName = "Invalid"),
};



/**
 *
 */
UENUM(BlueprintType)
enum class ETimestampCooldownStatus : uint8
{
	Once UMETA(DisplayName = "Once"),
    Pending UMETA(DisplayName = "Pending"),
	Completed UMETA(DisplayName = "Completed"),
	NotFound UMETA(DisplayName = "Not Found"),
};

