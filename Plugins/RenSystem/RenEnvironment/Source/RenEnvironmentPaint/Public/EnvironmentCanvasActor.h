// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEnvironmentPaint/Public/EnvironmentPaintConstant.h"

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

	UPROPERTY(EditAnywhere, Meta = (UIMin = "5.0", UIMax = "60.0", ClampMin = "5.0", ClampMax = "60.0"))
	float ScanInterval = 10.0f;

	UPROPERTY(EditAnywhere, Meta = (UIMin = "250.0", UIMax = "3500.0", ClampMin = "250.0", ClampMax = "3500.0"))
	float ScanDistance = 1000.0f;


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
	int MaxBrushes = 16;


	UPROPERTY(EditAnywhere, AdvancedDisplay)
	FEnvironmentCanvasParameters CanvasParameters;


	TArray<TWeakInterfacePtr<IEnvironmentBrushInterface>> PendingBrushAdd;
	TArray<TWeakInterfacePtr<IEnvironmentBrushInterface>> PendingBrushRemove;
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


	virtual void FindNearbyBrushes();
	virtual void ClearBrushComponents();
	virtual void GetBrushComponents(AActor* Actor, TArray<UActorComponent*>& OutComponents);

	void ResolvePendingBrushes();

	FTimerHandle BrushScanTimer;
	void StartBrushScan();
	void StopBrushScan();


	virtual void RegisterBrush(AActor* Actor);


	virtual FVector2D MakePixelOffset() const;
	virtual void MoveRenderTargets();
	virtual void DrawRenderTargets();

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditAnywhere, AdvancedDisplay)
	bool bDrawDebug = false;

	virtual void DrawDebug();

#endif

};

