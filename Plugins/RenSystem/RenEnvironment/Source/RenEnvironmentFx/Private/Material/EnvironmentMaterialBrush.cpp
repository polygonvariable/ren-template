// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Material/EnvironmentMaterialBrush.h"

// Engine Headers

// Project Headers



bool UEnvironmentMaterialBrush::GetBrushDetails(FVector& Location, FVector2D& Size, float& Density)
{
	const FTransform& Transform = GetComponentToWorld();
	Location = Transform.GetLocation();
	Size = BrushSize;

	return bCanDraw;
}

