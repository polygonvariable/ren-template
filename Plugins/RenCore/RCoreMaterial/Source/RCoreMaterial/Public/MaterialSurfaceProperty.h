// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Materials/MaterialParameterCollectionInstance.h"

// Forward Declarations
#include "MaterialSurfaceProperty.generated.h"



/**
 * 
 * 
 */
USTRUCT(BlueprintType)
struct FMaterialSurfaceProperty
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

	bool IsValid() const
	{
		bool bSpecular = (Specular > -1.0f && Specular <= 1.0f);
		bool bRoughness = (Roughness > -1.0f && Roughness <= 1.0f);
		bool bOpacity = (Opacity > -1.0f && Opacity <= 1.0f);
		
		return (bSpecular && bRoughness && bOpacity);
	}

    void GetParameters(UMaterialParameterCollectionInstance* Instance, FName TintName, FName SpecularName, FName RoughnessName, FName OpacityName)
    {
        Instance->GetVectorParameterValue(TintName, Tint);
		Instance->GetScalarParameterValue(SpecularName, Specular);
		Instance->GetScalarParameterValue(RoughnessName, Roughness);
		Instance->GetScalarParameterValue(OpacityName, Opacity);
    }

};