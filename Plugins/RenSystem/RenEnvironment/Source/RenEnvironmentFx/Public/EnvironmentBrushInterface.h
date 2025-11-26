// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "EnvironmentBrushInterface.generated.h"

// Forward Declarations

// Delegates Declarations



UINTERFACE(MinimalAPI, Blueprintable)
class UEnvironmentBrushInterface : public UInterface
{
	GENERATED_BODY()
};

class RENENVIRONMENTFX_API IEnvironmentBrushInterface
{
	GENERATED_BODY()

public:

	virtual bool GetBrushDetails(FVector& Location, FVector2D& Size, float &Density) { return false; };
	virtual bool GetBrushDetails(FVector& Location, FVector2D& Size, UMaterialInstanceDynamic*& Material) { return false; };
	
};

