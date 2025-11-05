// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "MaterialSurfaceProperty.h"

// Engine Headers
#include "Materials/MaterialParameterCollectionInstance.h"

// Project Header



bool FMaterialSurfaceProperty::IsValid() const
{
    bool bSpecular = (Specular > -1.0f && Specular <= 1.0f);
    bool bRoughness = (Roughness > -1.0f && Roughness <= 1.0f);
    bool bOpacity = (Opacity > -1.0f && Opacity <= 1.0f);
    
    return (bSpecular && bRoughness && bOpacity);
}

void FMaterialSurfaceProperty::FillParameters(UMaterialParameterCollectionInstance* Instance, FName TintName, FName SpecularName, FName RoughnessName, FName OpacityName)
{
    Instance->GetVectorParameterValue(TintName, Tint);
    Instance->GetScalarParameterValue(SpecularName, Specular);
    Instance->GetScalarParameterValue(RoughnessName, Roughness);
    Instance->GetScalarParameterValue(OpacityName, Opacity);
}

