// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEnvironmentFx/Public/EnvironmentCanvasActor.h"

// Generated Headers
#include "EnvironmentNiagaraCanvas.generated.h"

// Forward Declarations
class USceneComponent;
class UNiagaraComponent;

class IEnvironmentBrushInterface;



/**
 * Uses Niagara Grid2D to paint in render targets
 *
 */
UCLASS()
class AEnvironmentNiagaraCanvas : public AEnvironmentCanvasActor
{

	GENERATED_BODY()

public:

	AEnvironmentNiagaraCanvas();


	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

protected:

	virtual bool InitializePixel() override;

	virtual void MoveRenderTargets();
	virtual void DrawRenderTargets();

};

