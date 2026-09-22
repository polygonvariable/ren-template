// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Components/SceneComponent.h"

// Generated Headers
#include "EnvironmentBrushComponent.generated.h"

// Forward Declarations
class UCharacterMovementComponent;


/*
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UEnvironmentBrushComponent : public USceneComponent
{

	GENERATED_BODY()

public:

	UEnvironmentBrushComponent();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLineTrace = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanDraw = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D BrushSize = FVector2D(4.0f, 4.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrushDensity = 1.0f;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDrawDebug = false;
#endif


	void RegisterBrush();
	void UnregisterBrush();

	void SetBrushDensity(float Density);
	void SetBrushSize(FVector2D Size);
	void SetCanDraw(bool bEnable);

	// ~ IEnvironmentBrushInterface
	virtual bool GetBrushDetails(FVector& Location, FVector2D& Size, float& Density);
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


	bool IsNearGround() const;

};

