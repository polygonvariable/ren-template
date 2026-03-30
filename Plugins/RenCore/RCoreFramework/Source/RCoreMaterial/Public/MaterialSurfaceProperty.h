// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Generated Headers
#include "MaterialSurfaceProperty.generated.h"

// Module Macros
#define REN_API RCOREMATERIAL_API



/**
 * 
 * 
 */
USTRUCT(BlueprintType)
struct REN_API FMaterialSurfaceProperty
{

    GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly)
    FLinearColor Tint = FColor::Transparent;

    UPROPERTY(EditDefaultsOnly, Meta = (UIMin = "-1", UIMax = "1", ClampMin = "-1", ClampMax = "1"))
    float Specular = 0.0f;

    UPROPERTY(EditDefaultsOnly, Meta = (UIMin = "-1", UIMax = "1", ClampMin = "-1", ClampMax = "1"))
    float Roughness = 0.0f;

    UPROPERTY(EditDefaultsOnly, Meta = (UIMin = "-1", UIMax = "1", ClampMin = "-1", ClampMax = "1"))
    float Opacity = 0.0f;

    void Reset();
    void Clamp();

    void GetParameters(UMaterialParameterCollectionInstance* Instance, FName TintName, FName SpecularName, FName RoughnessName, FName OpacityName);
    void SetParameters(UMaterialParameterCollectionInstance* Instance, FName TintName, FName SpecularName, FName RoughnessName, FName OpacityName);

    static FMaterialSurfaceProperty Lerp(const FMaterialSurfaceProperty& A, const FMaterialSurfaceProperty& B, float Alpha);

};



// Module Macros
#undef REN_API

