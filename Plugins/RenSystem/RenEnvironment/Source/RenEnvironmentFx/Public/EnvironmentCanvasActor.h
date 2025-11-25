// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers
#include "EnvironmentCanvasActor.generated.h"

// Forward Declarations
class IEnvironmentBrushInterface;



/**
 * TODO:
 * Rename to EnvironmentCanvasActor
 * Can affect performance with too many brushes, use EnvironmentComputeCanvas & EnvironmentComputeBrush instead.
 * 
 */
UCLASS()
class AEnvironmentCanvasActor : public AActor
{

	GENERATED_BODY()

public:

	AEnvironmentCanvasActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CanvasSize = 4096.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ImageSize = 512.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float PixelRatio = 1.0f;



	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialParameterCollection> MPC;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialParameterCollectionInstance> MPCInstance;

	UPROPERTY(VisibleAnywhere)
	FVector2D PixelOffset;
	


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTextureRenderTarget2D> MainRenderTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTextureRenderTarget2D> PersistentRenderTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> DrawPersistentMaterial;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DrawPersistentMaterialInstance;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> DrawBrushMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> DrawMainAdditiveMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName BrushTag = TEXT("EnvironmentBrush");

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxBrushes = 16;

	TArray<TWeakInterfacePtr<IEnvironmentBrushInterface>> BrushCollection;


	void FindBrushComponents();

	void RegisterBrush(AActor* Actor);
	void UnregisterBrush(AActor* Actor);


	void MoveRenderTargets();
	void DrawPersistentRenderTarget();
	void DrawMainRenderTarget();

};

