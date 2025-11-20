// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "MaterialSurfaceProperty.h"

// Engine Headers
#include "Materials/MaterialParameterCollectionInstance.h"

// Project Header



void FMaterialSurfaceProperty::Reset()
{
	Tint = FColor::Transparent;
	Specular = 0.0f;
	Roughness = 0.0f;
	Opacity = 0.0f;
}

void FMaterialSurfaceProperty::Clamp()
{
	Specular = FMath::Clamp(Specular, -1.0f, 1.0f);
	Roughness = FMath::Clamp(Roughness, -1.0f, 1.0f);
	Opacity = FMath::Clamp(Opacity, -1.0f, 1.0f);
}

void FMaterialSurfaceProperty::GetParameters(UMaterialParameterCollectionInstance* Instance, FName TintName, FName SpecularName, FName RoughnessName, FName OpacityName)
{
    if (!Instance) return;

    Instance->GetVectorParameterValue(TintName, Tint);
    Instance->GetScalarParameterValue(SpecularName, Specular);
    Instance->GetScalarParameterValue(RoughnessName, Roughness);
    Instance->GetScalarParameterValue(OpacityName, Opacity);
}

void FMaterialSurfaceProperty::SetParameters(UMaterialParameterCollectionInstance* Instance, FName TintName, FName SpecularName, FName RoughnessName, FName OpacityName)
{
    if (!Instance) return;

	Instance->SetVectorParameterValue(TintName, Tint);
	Instance->SetScalarParameterValue(SpecularName, Specular);
	Instance->SetScalarParameterValue(RoughnessName, Roughness);
	Instance->SetScalarParameterValue(OpacityName, Opacity);
}

FMaterialSurfaceProperty FMaterialSurfaceProperty::Lerp(const FMaterialSurfaceProperty& A, const FMaterialSurfaceProperty& B, float Alpha)
{
    FMaterialSurfaceProperty Result;

	Result.Tint = FMath::Lerp(A.Tint, B.Tint, Alpha);
	Result.Specular = FMath::Lerp(A.Specular, B.Specular, Alpha);
	Result.Roughness = FMath::Lerp(A.Roughness, B.Roughness, Alpha);
	Result.Opacity = FMath::Lerp(A.Opacity, B.Opacity, Alpha);

	Result.Clamp();

	return Result;
}

