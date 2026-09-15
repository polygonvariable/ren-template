// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "MaterialLibrary.h"

// Engine Headers
#include "Materials/MaterialParameterCollectionInstance.h"


void FMaterialLibrary::GetSurfaceProperty(FMaterialSurfaceProperty& Surface, UMaterialParameterCollectionInstance* Instance, FName TintName, FName SROWName, FName DCMAName)
{
	if (!IsValid(Instance))
	{
		return;
	}

	FLinearColor TintColor;
	Instance->GetVectorParameterValue(TintName, TintColor);

	Surface.ColorAdd = TintColor;
	Surface.ColorMultiply = TintColor.A;

	FLinearColor SROW;
	Instance->GetVectorParameterValue(SROWName, SROW);

	Surface.Specular = SROW.R;
	Surface.Roughness = SROW.G;
	Surface.Opacity = SROW.B;
	Surface.WPO = SROW.A;

	FLinearColor DCMA;
	Instance->GetVectorParameterValue(DCMAName, DCMA);

	Surface.Displacement = DCMA.R;
	Surface.Coverage = DCMA.G;
	Surface.MaskType = DCMA.B;
	Surface.MaskAdd = DCMA.A;
}

void FMaterialLibrary::SetSurfaceProperty(const FMaterialSurfaceProperty& Surface, UMaterialParameterCollectionInstance* Instance, FName TintName, FName SROWName, FName DCMAName)
{
	if (!IsValid(Instance))
	{
		return;
	}

	Instance->SetVectorParameterValue(TintName, FVector4(Surface.ColorAdd.R, Surface.ColorAdd.G, Surface.ColorAdd.B, Surface.ColorMultiply));
	Instance->SetVectorParameterValue(SROWName, FVector4(Surface.Specular, Surface.Roughness, Surface.Opacity, Surface.WPO));
	Instance->SetVectorParameterValue(DCMAName, FVector4(Surface.Displacement, Surface.Coverage, Surface.MaskType, Surface.MaskAdd));
}

void FMaterialLibrary::LerpSurfaceProperty(const FMaterialSurfaceProperty& A, const FMaterialSurfaceProperty& B, float Alpha, FMaterialSurfaceProperty& OutResult)
{
	OutResult.ColorAdd = FMath::Lerp(A.ColorAdd, B.ColorAdd, Alpha);
	OutResult.ColorMultiply = FMath::Lerp(A.ColorMultiply, B.ColorMultiply, Alpha);

	OutResult.Specular = FMath::Lerp(A.Specular, B.Specular, Alpha);
	OutResult.Roughness = FMath::Lerp(A.Roughness, B.Roughness, Alpha);
	OutResult.Opacity = FMath::Lerp(A.Opacity, B.Opacity, Alpha);
	OutResult.WPO = FMath::Lerp(A.WPO, B.WPO, Alpha);

	OutResult.Displacement = FMath::Lerp(A.Displacement, B.Displacement, Alpha);
	OutResult.Coverage = FMath::Lerp(A.Coverage, B.Coverage, Alpha);
	OutResult.MaskType = FMath::Lerp(A.MaskType, B.MaskType, Alpha);
	OutResult.MaskAdd = FMath::Lerp(A.MaskAdd, B.MaskAdd, Alpha);

	OutResult.Clamp();
}

