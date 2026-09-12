// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/WorldSubsystem.h"
#include "Components/SceneComponent.h"
#include "NiagaraTickBehaviorEnum.h"

// Project Headers
#include "EnvironmentPaintConstant.h"

// Generated Headers
#include "EnvironmentCanvasSubsystem.generated.h"

// Forward Declarations
class IConsoleVariable;
class UNiagaraComponent;
class UNiagaraSystem;
class IEnvironmentBrushInterface;
class UTextureRenderTarget2D;
class UMaterialParameterCollection;
class UMaterialParameterCollectionInstance;
class UEnvironmentPaintWorldConfig;
struct FStreamableHandle;


/**
 *
 */
UCLASS(MinimalAPI)
class UEnvironmentCanvasSubsystem : public UTickableWorldSubsystem
{

	GENERATED_BODY()

public:

	void RegisterBrush(AActor* Actor);
	void RegisterBrush(UActorComponent* Component);

	void UnregisterBrush(AActor* Actor);
	void UnregisterBrush(UActorComponent* Component);

	// ~ UTickableWorldSubsystem
	virtual TStatId GetStatId() const override;
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	// ~ End of UTickableWorldSubsystem

	// ~ UWorldSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void OnWorldEndPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;
	// ~ End of UWorldSubsystem

protected:

	bool bIsDrawing = false;
	bool bCreatedSuccessfully = false;

	TSharedPtr<FStreamableHandle> StreamHandle;
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	TArray<TWeakInterfacePtr<IEnvironmentBrushInterface>> BrushToAdd;
	TArray<TWeakInterfacePtr<IEnvironmentBrushInterface>> BrushToRemove;
	TArray<TWeakInterfacePtr<IEnvironmentBrushInterface>> BrushCollection;

	TObjectPtr<UMaterialParameterCollectionInstance> MPCInstance;
	float PixelRatio = 1.0f;
	FVector2D PixelOffset;

	FVector NiagaraLocation;
	FEnvironmentCanvasParameters CanvasParameters;
	int CanvasSize;
	int RenderTargetSize;
	int BrushLimit = 32;
	int PointLimit = 16;

	TWeakObjectPtr<APlayerController> Controller;
	TWeakObjectPtr<APawn> Pawn;

	IConsoleVariable* EnvironmentPaintCVar;

#if WITH_EDITOR
	void DrawDebug();
#endif

	void ResolvePendingBrushes();

	void MoveRenderTargets();
	void DrawRenderTargets();

	bool InitializeNiagara(UNiagaraSystem* System, ENiagaraTickBehavior TickBehaviour);
	void DeinitializeNiagara();

	bool InitializeMPC(const UMaterialParameterCollection* Collection);
	bool InitializePixelRatio();
	bool InitializeRenderTargets(UTextureRenderTarget2D* MainRT, UTextureRenderTarget2D* PersistentRT);

	bool InitializeController();
	void DeinitializeController();

	FVector2D GetPixelOffset() const;
	const UEnvironmentPaintWorldConfig* GetPainWorldConfig(UWorld* World) const;

	// ~ Binding
	UFUNCTION()
	void HandleOnPawnChanged(APawn* OldPawn, APawn* NewPawn);
	void HandleOnEnvironmentCanvasLoaded();
	void HandleOnEnvironmentCVarChanged(IConsoleVariable* Variable);
	// ~ End of Binding
	
	// ~ UWorldSubsystem
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	// ~ End of UWorldSubsystem

public:

	static UEnvironmentCanvasSubsystem* Get(UWorld* World);

};

