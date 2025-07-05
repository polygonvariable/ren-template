// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Generated Headers
#include "EnvironmentProfileType.generated.h"

/**
 *
 */
UENUM(BlueprintType, DisplayName = "Environment Profile Type")
enum EEnvironmentProfileType : uint8
{
    Fog UMETA(DisplayName = "Fog"),
    Atmosphere UMETA(DisplayName = "Atmosphere"),
    Light UMETA(DisplayName = "Light"),
};

