// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Niagara/EnvironmentNiagaraCanvas.h"

// Engine Headers
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Engine/Canvas.h"
#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"

// Project Headers
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenEnvironmentFx/Public/EnvironmentBrushInterface.h"
#include "RenEnvironmentFx/Public/EnvironmentBrushComponent.h"




AEnvironmentNiagaraCanvas::AEnvironmentNiagaraCanvas()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	if (IsValid(SceneComponent))
	{
		SetRootComponent(SceneComponent);

		NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
		if (IsValid(NiagaraComponent))
		{
			NiagaraComponent->SetupAttachment(SceneComponent);
			NiagaraComponent->bAutoActivate = true;
		}
	}

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	bIsSpatiallyLoaded = false;
}


void AEnvironmentNiagaraCanvas::FindBrushComponents()
{
	UWorld* World = GetWorld();
	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		RegisterBrush(*ActorItr);
	}
}

void AEnvironmentNiagaraCanvas::ClearBrushComponents()
{
	BrushCollection.Empty();
}

void AEnvironmentNiagaraCanvas::GetBrushComponents(AActor* Actor, TArray<UActorComponent*>& OutComponents)
{
	OutComponents = Actor->GetComponentsByInterface(UEnvironmentBrushInterface::StaticClass());
}

void AEnvironmentNiagaraCanvas::RegisterBrush(AActor* Actor)
{
	int Num = BrushCollection.Num();
	if (MaxBrushes > 0 && Num >= MaxBrushes)
	{
		LOG_WARNING(LogEnvironment, TEXT("Max brushes reached"));
		return;
	}

	if (!IsValid(Actor) || !Actor->ActorHasTag(BrushTag))
	{
		LOG_ERROR(LogEnvironment, TEXT("Actor is invalid or not a brush"));
		return;
	}

	TArray<UActorComponent*> Components;
	GetBrushComponents(Actor, Components);

	for (UActorComponent* Component : Components)
	{
		IEnvironmentBrushInterface* Interface = Cast<IEnvironmentBrushInterface>(Component);
		if (Interface)
		{
			BrushCollection.Add(TWeakInterfacePtr<IEnvironmentBrushInterface>(Interface));
		}
	}
}

void AEnvironmentNiagaraCanvas::UnregisterBrush(AActor* Actor)
{
	if (!IsValid(Actor))
	{
		LOG_ERROR(LogEnvironment, TEXT("Actor is invalid or not a brush"));
		return;
	}

	TArray<UActorComponent*> Components;
	GetBrushComponents(Actor, Components);

	for (UActorComponent* Component : Components)
	{
		IEnvironmentBrushInterface* Interface = Cast<IEnvironmentBrushInterface>(Component);
		if (Interface)
		{
			BrushCollection.Remove(Interface);
		}
	}
}



void AEnvironmentNiagaraCanvas::Initialize()
{
	if (!MPC || !IsValid(NiagaraComponent))
	{
		LOG_ERROR(LogEnvironment, TEXT("MPC is invalid"));
		return;
	}

	InitializeMPC();
	InitializePixel();
	InitializeController();
	InitializeRenderTargets();

	FindBrushComponents();

	SetActorTickEnabled(true);
}

void AEnvironmentNiagaraCanvas::Deinitialize()
{
	SetActorTickEnabled(false);

	DeinitializeController();

	ClearBrushComponents();
}


void AEnvironmentNiagaraCanvas::InitializeMPC()
{
	UWorld* World = GetWorld();

	MPCInstance = World->GetParameterCollectionInstance(MPC);
	if (!IsValid(MPCInstance))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("MPCInstance is invalid"));
		return;
	}

	MPCInstance->SetScalarParameterValue(TEXT("Canvas.Size"), CanvasSize);
	MPCInstance->SetVectorParameterValue(TEXT("Canvas.Location"), GetActorLocation());
}

void AEnvironmentNiagaraCanvas::InitializePixel()
{
	if (CanvasSize <= 0.0f || ImageSize <= 0.0f)
	{
		LOG_ERROR(LogEnvironment, TEXT("CanvasSize or ImageSize is invalid"));
		return;
	}

	PixelRatio = (1.0f / ImageSize) * CanvasSize;
}

void AEnvironmentNiagaraCanvas::InitializeRenderTargets()
{
	if (!MainRenderTarget || !PersistentRenderTarget)
	{
		LOG_ERROR(LogEnvironment, TEXT("MainRT, PersistentRT is invalid"));
		return;
	}

	FLinearColor ClearColor = FLinearColor::Black;
	UKismetRenderingLibrary::ClearRenderTarget2D(this, MainRenderTarget, ClearColor);
	UKismetRenderingLibrary::ClearRenderTarget2D(this, PersistentRenderTarget, ClearColor);
}

void AEnvironmentNiagaraCanvas::InitializeController()
{
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();

	if (!IsValid(PlayerController))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("PlayerController is invalid"));
		return;
	}

	PlayerController->OnPossessedPawnChanged.AddDynamic(this, &AEnvironmentNiagaraCanvas::InitializeCharacter);

	Controller = PlayerController;
	Pawn = PlayerController->GetPawn();
}

void AEnvironmentNiagaraCanvas::DeinitializeController()
{
	APlayerController* PlayerController = Controller.Get();
	if (IsValid(PlayerController))
	{
		PlayerController->OnPossessedPawnChanged.RemoveAll(this);
	}

	Controller = nullptr;
	Pawn = nullptr;
}


void AEnvironmentNiagaraCanvas::InitializeCharacter(APawn* OldPawn, APawn* NewPawn)
{
	Pawn = NewPawn;
}


void AEnvironmentNiagaraCanvas::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void AEnvironmentNiagaraCanvas::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveRenderTargets();
	DrawRenderTargets();

#if WITH_EDITOR

	DrawDebug();

#endif
}

void AEnvironmentNiagaraCanvas::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Deinitialize();

	Super::EndPlay(EndPlayReason);
}


void AEnvironmentNiagaraCanvas::MoveRenderTargets()
{
	APawn* PlayerCharacter = Pawn.Get();
	if (!MPCInstance || !IsValid(PlayerCharacter) || !IsValid(NiagaraComponent))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("MPCInstance, Pawn, NiagaraComponent is invalid"));
		return;
	}

	FVector CurrentLocation = GetActorLocation();
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();

	float X = FMath::Floor(PlayerLocation.X / PixelRatio);
	float Y = FMath::Floor(PlayerLocation.Y / PixelRatio);

	FVector2D Snap2D = (FVector2D(X, Y) + 0.5f) * PixelRatio;
	FVector2D Location2D = FVector2D(CurrentLocation.X, CurrentLocation.Y);

	PixelOffset = Snap2D - Location2D;

	FVector2D Offset = FVector2D((PixelOffset.X / CanvasSize), (PixelOffset.Y / CanvasSize));

	FName Name = TEXT("User.Offset");
	NiagaraComponent->SetVariableVec2(Name, Offset);

	AddActorWorldOffset(FVector(PixelOffset.X, PixelOffset.Y, 0.0f));

	MPCInstance->SetVectorParameterValue(TEXT("Canvas.Location"), GetActorLocation());
}

void AEnvironmentNiagaraCanvas::DrawRenderTargets()
{
	TArray<FVector4> BrushPoints;

	FVector CurrentLocation = GetActorLocation();

	for (TWeakInterfacePtr<IEnvironmentBrushInterface>& Interface : BrushCollection)
	{
		IEnvironmentBrushInterface* Brush = Interface.Get();

		if (!Brush)
		{
			continue;
		}

		FVector Location = FVector::ZeroVector;
		FVector2D Size = FVector2D::ZeroVector;
		float Density = 0.0f;

		bool bCanDraw = Brush->GetBrushDetails(Location, Size, Density);
		if (!bCanDraw)
		{
			continue;
		}

		FVector DrawLocation = Location - CurrentLocation;

		BrushPoints.Add(FVector4(DrawLocation.X, DrawLocation.Y, Size.Length(), Density));
	}
	
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector4(NiagaraComponent, TEXT("User.DrawPoints"), BrushPoints);
}

#if WITH_EDITOR

void AEnvironmentNiagaraCanvas::DrawDebug()
{
	UWorld* World = GetWorld();
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + FVector(0.0f, 0.0f, 100.0f);

	DrawDebugDirectionalArrow(World, StartLocation, EndLocation, 10.0f, FColor::Magenta, false, 0.0f, 0, 20.0f);
}

#endif

