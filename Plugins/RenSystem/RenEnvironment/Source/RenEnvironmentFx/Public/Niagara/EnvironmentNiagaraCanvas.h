// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

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
class AEnvironmentNiagaraCanvas : public AActor
{

	GENERATED_BODY()

public:

	AEnvironmentNiagaraCanvas();


	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;


	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CanvasSize = 4096.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ImageSize = 512.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, AdvancedDisplay)
	float PixelRatio = 1.0f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTextureRenderTarget2D> MainRenderTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTextureRenderTarget2D> PersistentRenderTarget;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialParameterCollection> MPC;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	TObjectPtr<UMaterialParameterCollectionInstance> MPCInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, AdvancedDisplay)
	FVector2D PixelOffset;


	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	TWeakObjectPtr<APlayerController> Controller;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	TWeakObjectPtr<APawn> Pawn;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName BrushTag = TEXT("Environment.Brush");

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxBrushes = 16;


	TArray<TWeakInterfacePtr<IEnvironmentBrushInterface>> BrushCollection;


	virtual void Initialize();
	virtual void Deinitialize();

	virtual void InitializeMPC();
	virtual void InitializePixel();
	virtual void InitializeRenderTargets();

	virtual void InitializeController();
	virtual void DeinitializeController();

	UFUNCTION()
	virtual void InitializeCharacter(APawn* OldPawn, APawn* NewPawn);

	virtual void FindBrushComponents();
	virtual void ClearBrushComponents();
	virtual void GetBrushComponents(AActor* Actor, TArray<UActorComponent*>& OutComponents);

	virtual void RegisterBrush(AActor* Actor);
	virtual void UnregisterBrush(AActor* Actor);

	virtual void MoveRenderTargets();
	virtual void DrawRenderTargets();

#if WITH_EDITOR

	virtual void DrawDebug();

#endif

};

