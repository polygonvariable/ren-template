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

	UPROPERTY(EditAnywhere)
	float CanvasSize = 4096.0f;

	UPROPERTY(EditAnywhere)
	float ImageSize = 512.0f;
	
	float PixelRatio = 1.0f;
	FVector2D PixelOffset;


	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialParameterCollection> MPC;

	TObjectPtr<UMaterialParameterCollectionInstance> MPCInstance;


	UPROPERTY(EditAnywhere)
	TObjectPtr<UTextureRenderTarget2D> MainRenderTarget;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTextureRenderTarget2D> PersistentRenderTarget;


	UPROPERTY(EditAnywhere)
	FName BrushTag = TEXT("Environment.Brush");

	UPROPERTY(EditAnywhere)
	int MaxBrushes = 16;


	TArray<TWeakInterfacePtr<IEnvironmentBrushInterface>> BrushCollection;

	TWeakObjectPtr<APlayerController> Controller;
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


	virtual FVector2D MakePixelOffset() const;
	virtual void MoveRenderTargets();
	virtual void DrawRenderTargets();

#if WITH_EDITOR

	virtual void DrawDebug();

#endif

};

