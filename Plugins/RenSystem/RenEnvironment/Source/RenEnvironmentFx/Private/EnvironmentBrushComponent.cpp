// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EnvironmentBrushComponent.h"

// Engine Headers

// Project Headers



bool UEnvironmentBrushComponent::GetBrushDetails(FVector& Location, FVector2D& Size)
{
	const FTransform& Transform = GetComponentToWorld();
	Location = Transform.GetLocation();
	Size = BrushSize;

	return bCanDraw;
}/*

bool UEnvironmentBrushComponent::GetBrushDetails(FVector& Location, FVector2D& Size, UMaterialInstanceDynamic*& Material)
{
	const FTransform& Transform = GetComponentToWorld();
	Location = Transform.GetLocation();
	Size = BrushSize;
	Material = BrushMaterialInstance;

	return bCanDraw;
}*/

void UEnvironmentBrushComponent::SetCanDraw(bool bEnable)
{
	bCanDraw = bEnable;
}

void UEnvironmentBrushComponent::SetBrushSize(FVector2D Size)
{
	BrushSize = Size;
}

