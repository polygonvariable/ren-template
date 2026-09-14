// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "NiagaraTickBehaviorEnum.h"

// Project Headers
#include "Core/Type/EnvironmentCanvasParameter.h"
#include "WorldConfigAsset.h"

// Generated Headers
#include "EnvironmentPaintWorldConfig.generated.h"

// Forward Declarations
class UNiagaraSystem;
class UTextureRenderTarget2D;
class UMaterialParameterCollection;


/**
 *
 */
UCLASS(MinimalAPI)
class UEnvironmentPaintWorldConfig : public UWorldConfigAsset
{

    GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Niagara")
	bool bEnabled = true;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	TSoftObjectPtr<UNiagaraSystem> NiagaraSystem;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	ENiagaraTickBehavior NiagaraTick = ENiagaraTickBehavior::ForceTickLast;

	UPROPERTY(EditAnywhere, Category = "Render Target")
	TSoftObjectPtr<UTextureRenderTarget2D> MainRT;

	UPROPERTY(EditAnywhere, Category = "Render Target")
	TSoftObjectPtr<UTextureRenderTarget2D> PersistentRT;

	UPROPERTY(EditAnywhere, Category = "Render Target")
	int RenderTargetSize = 512;

	UPROPERTY(EditAnywhere, Category = "Canvas")
	int CanvasSize = 4096;

	UPROPERTY(EditAnywhere, Category = "Canvas")
	FEnvironmentCanvasParameter CanvasParameter;

	/** Num of brushes interfaces that can be registered */
	UPROPERTY(EditAnywhere, Category = "Brush")
	int BrushLimit = 32;

	/** Num of brushes data that can passed to gpu */
	UPROPERTY(EditAnywhere, Category = "Brush")
	int PointLimit = 16;

	UPROPERTY(EditAnywhere, Category = "Material")
	TSoftObjectPtr<UMaterialParameterCollection> MPC;


#if WITH_EDITORONLY_DATA
	// ~ UPrimaryDataAsset
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
	// ~ End of UPrimaryDataAsset
#endif

};

