// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "MaterialSurfaceProperty.h"

// Engine Headers
#include "Materials/MaterialParameterCollectionInstance.h"


void FMaterialSurfaceProperty::Reset()
{
	ColorAdd = FColor::Transparent;
	ColorMultiply = 1.0f;

	Specular = 0.0f;
	Roughness = 0.0f;
	Opacity = 0.0f;
	WPO = 1.0f;

	Displacement = 1.0f;
	Coverage = 0.0f;
	MaskType = 0.0f;
	MaskAdd = 0.0f;
}

void FMaterialSurfaceProperty::Clamp()
{
	ColorMultiply = FMath::Clamp(ColorMultiply, 0.0f, 1.0f);

	Specular = FMath::Clamp(Specular, -1.0f, 1.0f);
	Roughness = FMath::Clamp(Roughness, -1.0f, 1.0f);
	Opacity = FMath::Clamp(Opacity, -1.0f, 1.0f);

	Coverage = FMath::Clamp(Coverage, -1.0f, 1.0f);
	MaskAdd = FMath::Clamp(MaskAdd, 0.0f, 1.0f);
}

void FMaterialSurfaceProperty::GetParameters(UMaterialParameterCollectionInstance* Instance, FName TintName, FName SROWName, FName DCMAName)
{
	if (!IsValid(Instance))
	{
		return;
	}

	FLinearColor TintColor;
    Instance->GetVectorParameterValue(TintName, TintColor);

	ColorAdd = TintColor;
	ColorMultiply = ColorAdd.A;

	FLinearColor SROW;
	Instance->GetVectorParameterValue(SROWName, SROW);

	Specular = SROW.R;
	Roughness = SROW.G;
	Opacity = SROW.B;
	WPO = SROW.A;

	FLinearColor DCMA;
	Instance->GetVectorParameterValue(DCMAName, DCMA);

	Displacement = DCMA.R;
	Coverage = DCMA.G;
	MaskType = DCMA.B;
	MaskAdd = DCMA.A;
}

void FMaterialSurfaceProperty::SetParameters(UMaterialParameterCollectionInstance* Instance, FName TintName, FName SROWName, FName DCMAName)
{
	if (!IsValid(Instance))
	{
		return;
	}

	Instance->SetVectorParameterValue(TintName, FVector4(ColorAdd.R, ColorAdd.G, ColorAdd.B, ColorMultiply));
	Instance->SetVectorParameterValue(SROWName, FVector4(Specular, Roughness, Opacity, WPO));
	Instance->SetVectorParameterValue(DCMAName, FVector4(Displacement, Coverage, MaskType, MaskAdd));
}

FMaterialSurfaceProperty FMaterialSurfaceProperty::Lerp(const FMaterialSurfaceProperty& A, const FMaterialSurfaceProperty& B, float Alpha)
{
    FMaterialSurfaceProperty Result;

	Result.ColorAdd = FMath::Lerp(A.ColorAdd, B.ColorAdd, Alpha);
	Result.ColorMultiply = FMath::Lerp(A.ColorMultiply, B.ColorMultiply, Alpha);

	Result.Specular = FMath::Lerp(A.Specular, B.Specular, Alpha);
	Result.Roughness = FMath::Lerp(A.Roughness, B.Roughness, Alpha);
	Result.Opacity = FMath::Lerp(A.Opacity, B.Opacity, Alpha);
	Result.WPO = FMath::Lerp(A.WPO, B.WPO, Alpha);

	Result.Displacement = FMath::Lerp(A.Displacement, B.Displacement, Alpha);
	Result.Coverage = FMath::Lerp(A.Coverage, B.Coverage, Alpha);
	Result.MaskType = FMath::Lerp(A.MaskType, B.MaskType, Alpha);
	Result.MaskAdd = FMath::Lerp(A.MaskAdd, B.MaskAdd, Alpha);

	Result.Clamp();

	return Result;
}

