// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"


/**
 *
 */
UENUM(BlueprintType, DisplayName = "Stamped Event Status")
enum EStampedEventStatus
{
	Changed UMETA(DisplayName = "Changed"),
	Unchanged UMETA(DisplayName = "Unchanged"),
};


/**
 *
 */
UENUM(BlueprintType, DisplayName = "Stamped Event Cooldown Status")
enum EStampedEventCooldownStatus
{
	Once UMETA(DisplayName = "Once"),
    Pending UMETA(DisplayName = "Pending"),
	Completed UMETA(DisplayName = "Completed"),
	NotFound UMETA(DisplayName = "Not Found"),
};

