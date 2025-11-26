// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEnvironmentFx/Public/EnvironmentCanvasActor.h"

// Generated Headers
#include "EnvironmentMaterialCanvas.generated.h"

// Forward Declarations
class IEnvironmentBrushInterface;



/**
 * TODO:
 * Rename to EnvironmentCanvasActor
 * Can affect performance with too many brushes, use EnvironmentComputeCanvas & EnvironmentComputeBrush instead.
 *
 */
UCLASS()
class AEnvironmentMaterialCanvas : public AEnvironmentCanvasActor
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> DrawPersistentMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstanceDynamic> DrawPersistentMaterialInstance;



	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> DrawBrushMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> DrawMainAdditiveMaterial;



	virtual bool Initialize() override;
	virtual bool InitializeRenderTargets() override;

	virtual void DrawRenderTargets() override;

	virtual void DrawPersistentRenderTarget();
	virtual void DrawMainRenderTarget();

};

