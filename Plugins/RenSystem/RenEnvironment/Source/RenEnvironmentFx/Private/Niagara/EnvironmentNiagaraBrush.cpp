// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Niagara/EnvironmentNiagaraBrush.h"

// Engine Headers

// Project Headers



bool UEnvironmentNiagaraBrush::GetBrushDetails(FVector& Location, FVector2D& Size, float& Density)
{
	const FTransform& Transform = GetComponentToWorld();
	Location = Transform.GetLocation();
	Size = BrushSize;
	Density = BrushDensity;

	return bCanDraw;
}

