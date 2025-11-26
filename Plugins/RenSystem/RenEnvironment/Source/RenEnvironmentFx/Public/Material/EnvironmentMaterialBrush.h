// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEnvironmentFx/Public/EnvironmentBrushComponent.h"

// Generated Headers
#include "EnvironmentMaterialBrush.generated.h"

// Forward Declarations



/*
 * Can affect performance with too many brushes, use ComputeCanvas & ComputeBrush instead.
 *
 * 
 */
UCLASS(ClassGroup = (Custom), Meta = (BlueprintSpawnableComponent))
class UEnvironmentMaterialBrush : public UEnvironmentBrushComponent
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> BrushMaterial;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> BrushMaterialInstance;


	virtual bool GetBrushDetails(FVector& Location, FVector2D& Size, float& Density) override;

};

