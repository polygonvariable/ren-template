// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Components/SceneComponent.h"

// Project Headers
#include "EnvironmentBrushInterface.h"

// Generated Headers
#include "EnvironmentBrushComponent.generated.h"

// Forward Declarations
class UCharacterMovementComponent;



/*
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UEnvironmentBrushComponent : public USceneComponent, public IEnvironmentBrushInterface
{

	GENERATED_BODY()

public:

	UEnvironmentBrushComponent();

	void RegisterBrush();
	void UnregisterBrush();

	void SetBrushDensity(float Density);
	void SetBrushSize(FVector2D Size);
	void SetCanDraw(bool bEnable);

	// ~ IEnvironmentBrushInterface
	virtual bool GetBrushDetails(FVector& Location, FVector2D& Size, float& Density) override;
	virtual bool GetBrushDetails(FVector& Location, FVector& Velocity, FVector2D& Size, float& Density) override;
	// ~ End of IEnvironmentBrushInterface

	// ~ USceneComponent
	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;
	// ~ End of USceneComponent

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

};

