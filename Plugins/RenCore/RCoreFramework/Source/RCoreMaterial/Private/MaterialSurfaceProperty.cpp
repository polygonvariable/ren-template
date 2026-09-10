// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "MaterialSurfaceProperty.h"

// Engine Headers
#include "Materials/MaterialParameterCollectionInstance.h"


void FMaterialSurfaceProperty::Reset()
{
	Tint = FColor::Transparent;
	Specular = 0.0f;
	Roughness = 0.0f;
	Opacity = 0.0f;
	WPO = 1.0f;
}

void FMaterialSurfaceProperty::Clamp()
{
	Specular = FMath::Clamp(Specular, -1.0f, 1.0f);
	Roughness = FMath::Clamp(Roughness, -1.0f, 1.0f);
	Opacity = FMath::Clamp(Opacity, -1.0f, 1.0f);
}

void FMaterialSurfaceProperty::GetParameters(UMaterialParameterCollectionInstance* Instance, FName TintName, FName SROWName)
{
	if (!IsValid(Instance))
	{
		return;
	}

    Instance->GetVectorParameterValue(TintName, Tint);

	FLinearColor SROW;
	Instance->GetVectorParameterValue(SROWName, SROW);

	Specular = SROW.R;
	Roughness = SROW.G;
	Opacity = SROW.B;
	WPO = SROW.A;
}

void FMaterialSurfaceProperty::SetParameters(UMaterialParameterCollectionInstance* Instance, FName TintName, FName SROWName)
{
	if (!IsValid(Instance))
	{
		return;
	}

	Instance->SetVectorParameterValue(TintName, Tint);
	Instance->SetVectorParameterValue(SROWName, FVector4(Specular, Roughness, Opacity, WPO));
}

FMaterialSurfaceProperty FMaterialSurfaceProperty::Lerp(const FMaterialSurfaceProperty& A, const FMaterialSurfaceProperty& B, float Alpha)
{
    FMaterialSurfaceProperty Result;

	Result.Tint = FMath::Lerp(A.Tint, B.Tint, Alpha);
	Result.Specular = FMath::Lerp(A.Specular, B.Specular, Alpha);
	Result.Roughness = FMath::Lerp(A.Roughness, B.Roughness, Alpha);
	Result.Opacity = FMath::Lerp(A.Opacity, B.Opacity, Alpha);
	Result.WPO = FMath::Lerp(A.WPO, B.WPO, Alpha);

	Result.Clamp();

	return Result;
}

