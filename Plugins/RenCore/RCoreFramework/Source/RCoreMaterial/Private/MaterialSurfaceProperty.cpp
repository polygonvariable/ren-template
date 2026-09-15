// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "MaterialSurfaceProperty.h"


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

