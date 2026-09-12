// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Generated Headers
#include "EnvironmentBrushInterface.generated.h"


/*
 *
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UEnvironmentBrushInterface : public UInterface
{
	GENERATED_BODY()
};

class IEnvironmentBrushInterface
{
	GENERATED_BODY()

public:

	virtual bool GetBrushDetails(FVector& Location, FVector2D& Size, float& Density) { return false; };
	virtual bool GetBrushDetails(FVector& Location, FVector& Velocity, FVector2D& Size, float& Density) { return false; };
	virtual bool GetBrushDetails(FVector& Location, FVector& Velocity, FVector2D& Size, UMaterialInstanceDynamic*& Material) { return false; };
	
};

