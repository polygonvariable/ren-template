// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

// Project Headers
#include "RenEnvironmentFx/Public/EnvironmentBrushInterface.h"

// Generated Headers
#include "EnvironmentBrushComponent.generated.h"

// Forward Declarations



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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanDraw = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D BrushSize = FVector2D(4.0f, 4.0f);


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> BrushMaterial;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> BrushMaterialInstance;


	virtual bool GetBrushDetails(FVector& Location, FVector2D& Size) override;
	//virtual bool GetBrushDetails(FVector& Location, FVector2D& Size, UMaterialInstanceDynamic*& Material) override;


	UFUNCTION(BlueprintCallable)
	void SetBrushSize(FVector2D Size);

	UFUNCTION(BlueprintCallable)
	void SetCanDraw(bool bEnable);

};

