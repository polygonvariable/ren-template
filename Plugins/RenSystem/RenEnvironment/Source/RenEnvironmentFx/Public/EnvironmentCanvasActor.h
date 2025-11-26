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
UCLASS(Abstract, MinimalAPI)
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

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialParameterCollectionInstance> MPCInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D PixelOffset;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTextureRenderTarget2D> MainRenderTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTextureRenderTarget2D> PersistentRenderTarget;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName BrushTag = TEXT("Environment.Brush");

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxBrushes = 16;

	TArray<TWeakInterfacePtr<IEnvironmentBrushInterface>> BrushCollection;


	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<APlayerController> Controller;

	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<APawn> Pawn;



	virtual bool Initialize();
	virtual void Deinitialize();

	virtual bool InitializeMPC();
	virtual bool InitializePixel();
	virtual bool InitializeRenderTargets();

	virtual bool InitializeController();
	virtual void DeinitializeController();

	UFUNCTION()
	virtual void UpdatePawn(APawn* OldPawn, APawn* NewPawn);


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

