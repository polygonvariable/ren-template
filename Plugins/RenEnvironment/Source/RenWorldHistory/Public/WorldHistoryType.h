// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"



/**
 *
 */
UENUM(BlueprintType)
enum EWorldHistoryStatus : uint8
{
	Changed UMETA(DisplayName = "Changed"),
	Unchanged UMETA(DisplayName = "Unchanged"),
};



/**
 *
 */
UENUM(BlueprintType)
enum EWorldHistoryCooldownStatus : uint8
{
	Once UMETA(DisplayName = "Once"),
    Pending UMETA(DisplayName = "Pending"),
	Completed UMETA(DisplayName = "Completed"),
	NotFound UMETA(DisplayName = "Not Found"),
};

