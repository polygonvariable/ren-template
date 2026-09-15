// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "MaterialSurfaceProperty.generated.h"

// Module Macros
#define REN_API RCOREMATERIAL_API


/**
 * 
 */
USTRUCT(BlueprintType)
struct REN_API FMaterialSurfaceProperty
{

    GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly, meta = (HideAlphaChannel))
    FLinearColor ColorAdd = FColor::Transparent;

    UPROPERTY(EditDefaultsOnly, meta = (UIMin = "0", UIMax = "1", ClampMin = "0", ClampMax = "1"))
    float ColorMultiply = 1.0f;

    UPROPERTY(EditDefaultsOnly, meta = (UIMin = "-1", UIMax = "1", ClampMin = "-1", ClampMax = "1"))
    float Specular = 0.0f;

    UPROPERTY(EditDefaultsOnly, meta = (UIMin = "-1", UIMax = "1", ClampMin = "-1", ClampMax = "1"))
    float Roughness = 0.0f;

    UPROPERTY(EditDefaultsOnly, meta = (UIMin = "-1", UIMax = "1", ClampMin = "-1", ClampMax = "1"))
    float Opacity = 0.0f;

    UPROPERTY(EditDefaultsOnly)
    float WPO = 1.0f;

    UPROPERTY(EditDefaultsOnly)
    float Displacement = 1.0f;

    UPROPERTY(EditDefaultsOnly, meta = (UIMin = "-1", UIMax = "1", ClampMin = "-1", ClampMax = "1"))
    float Coverage = 0.0f;

    UPROPERTY(EditDefaultsOnly)
    float MaskType = 0.0f;

    UPROPERTY(EditDefaultsOnly, meta = (UIMin = "0", UIMax = "1", ClampMin = "0", ClampMax = "1"))
    float MaskAdd = 0.0f;


    void Reset();
    void Clamp();

};


// Module Macros
#undef REN_API

