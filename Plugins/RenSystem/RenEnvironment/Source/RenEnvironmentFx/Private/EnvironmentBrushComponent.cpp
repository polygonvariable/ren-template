// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EnvironmentBrushComponent.h"

// Engine Headers

// Project Headers



bool UEnvironmentBrushComponent::GetBrushDetails(FVector& Location, FVector2D& Size, float& Density)
{
	const FTransform& Transform = GetComponentToWorld();
	Location = Transform.GetLocation();
	Size = BrushSize;
	Density = BrushDensity;

	return bCanDraw;
}

void UEnvironmentBrushComponent::SetCanDraw(bool bEnable)
{
	bCanDraw = bEnable;
}

void UEnvironmentBrushComponent::SetBrushSize(FVector2D Size)
{
	BrushSize = Size;
}

