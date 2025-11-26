// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEnvironmentFx/Public/EnvironmentBrushComponent.h"

// Generated Headers
#include "EnvironmentNiagaraBrush.generated.h"

// Forward Declarations



/*
 * Can affect performance with too many brushes, use ComputeCanvas & ComputeBrush instead.
 *
 *
 */
UCLASS(ClassGroup = (Custom), Meta = (BlueprintSpawnableComponent))
class UEnvironmentNiagaraBrush : public UEnvironmentBrushComponent
{

	GENERATED_BODY()

public:

	virtual bool GetBrushDetails(FVector& Location, FVector2D& Size, float& Density) override;

};

