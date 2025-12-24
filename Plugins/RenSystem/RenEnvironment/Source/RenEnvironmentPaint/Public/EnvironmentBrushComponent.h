// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

// Project Headers
#include "RenEnvironmentPaint/Public/EnvironmentBrushInterface.h"

// Generated Headers
#include "EnvironmentBrushComponent.generated.h"

// Forward Declarations
class UCharacterMovementComponent;



/*
 * TODO:
 * Rename to EnvironmentBrushComponent
 * Can affect performance with too many brushes, use ComputeCanvas & ComputeBrush instead.
 * 
 */
UCLASS(ClassGroup = (Custom), Meta = (BlueprintSpawnableComponent))
class UEnvironmentBrushComponent : public USceneComponent, public IEnvironmentBrushInterface
{

	GENERATED_BODY()

public:

	void SetBrushDensity(float Density);
	void SetBrushSize(FVector2D Size);
	void SetCanDraw(bool bEnable);

protected:

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;

	UPROPERTY()
	bool bIsCharacter = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanDraw = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D BrushSize = FVector2D(4.0f, 4.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrushDensity = 1.0f;

public:

	// ~ USceneComponent
	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;
	// ~ End of USceneComponent

	// ~ IEnvironmentBrushInterface
	virtual bool GetBrushDetails(FVector& Location, FVector& Velocity, FVector2D& Size, float& Density) override;
	// ~ End of IEnvironmentBrushInterface

};

