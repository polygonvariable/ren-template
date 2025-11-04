// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Generated Headers
#include "EnvironmentProfileType.generated.h"



/**
 *
 */
UENUM(BlueprintType)
enum class EEnvironmentProfileType : uint8
{
    Default UMETA(DisplayName = "Default"),
    Fog UMETA(DisplayName = "Fog"),
    Atmosphere UMETA(DisplayName = "Atmosphere"),
    Light UMETA(DisplayName = "Light"),
};

