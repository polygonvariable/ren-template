// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EnvironmentCanvasActor.h"

// Engine Headers
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Engine/Canvas.h"
#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "Materials/MaterialParameterCollectionInstance.h"

// Project Headers
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenEnvironmentFx/Public/EnvironmentBrushInterface.h"
#include "RenEnvironmentFx/Public/EnvironmentBrushComponent.h"
#include "RenEnvironmentFx/Public/EnvironmentPaintConstant.h"



AEnvironmentCanvasActor::AEnvironmentCanvasActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	bIsSpatiallyLoaded = false;
}


void AEnvironmentCanvasActor::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void AEnvironmentCanvasActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveRenderTargets();
	DrawRenderTargets();

#if WITH_EDITOR

	DrawDebug();

#endif
}

void AEnvironmentCanvasActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Deinitialize();

	Super::EndPlay(EndPlayReason);
}


void AEnvironmentCanvasActor::FindBrushComponents()
{
	UWorld* World = GetWorld();
	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		RegisterBrush(*ActorItr);
	}
}

void AEnvironmentCanvasActor::ClearBrushComponents()
{
	BrushCollection.Empty();
}

void AEnvironmentCanvasActor::GetBrushComponents(AActor* Actor, TArray<UActorComponent*>& OutComponents)
{
	OutComponents = Actor->GetComponentsByInterface(UEnvironmentBrushInterface::StaticClass());
}

void AEnvironmentCanvasActor::RegisterBrush(AActor* Actor)
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

void AEnvironmentCanvasActor::UnregisterBrush(AActor* Actor)
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


bool AEnvironmentCanvasActor::Initialize()
{
	if (!MPC || !MainRenderTarget || !PersistentRenderTarget)
	{
		LOG_ERROR(LogEnvironment, TEXT("Invalid parameters"));
		return false;
	}

	InitializeMPC();
	InitializePixel();
	InitializeController();
	InitializeRenderTargets();

	FindBrushComponents();

	SetActorTickEnabled(true);

	return true;
}

void AEnvironmentCanvasActor::Deinitialize()
{
	SetActorTickEnabled(false);

	DeinitializeController();

	ClearBrushComponents();
}


bool AEnvironmentCanvasActor::InitializeMPC()
{
	UWorld* World = GetWorld();

	MPCInstance = World->GetParameterCollectionInstance(MPC);
	if (!IsValid(MPCInstance))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("MPCInstance is invalid"));
		return false;
	}

	MPCInstance->SetScalarParameterValue(EnvironmentPaintConstant::MPC_CanvasSize, CanvasSize);
	MPCInstance->SetVectorParameterValue(EnvironmentPaintConstant::MPC_CanvasLocation, GetActorLocation());

	return true;
}

bool AEnvironmentCanvasActor::InitializePixel()
{
	if (CanvasSize <= 0.0f || ImageSize <= 0.0f)
	{
		LOG_ERROR(LogEnvironment, TEXT("CanvasSize or ImageSize is invalid"));
		return false;
	}

	PixelRatio = CanvasSize / ImageSize;

	return true;
}

bool AEnvironmentCanvasActor::InitializeRenderTargets()
{
	if (!MainRenderTarget || !PersistentRenderTarget)
	{
		LOG_ERROR(LogEnvironment, TEXT("PersistentMaterial, MainRT, PersistentRT is invalid"));
		return false;
	}

	FLinearColor ClearColor = FLinearColor::Black;
	UKismetRenderingLibrary::ClearRenderTarget2D(this, MainRenderTarget, ClearColor);
	UKismetRenderingLibrary::ClearRenderTarget2D(this, PersistentRenderTarget, ClearColor);

	return true;
}


bool AEnvironmentCanvasActor::InitializeController()
{
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();

	if (!IsValid(PlayerController))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("PlayerController is invalid"));
		return false;
	}

	PlayerController->OnPossessedPawnChanged.AddDynamic(this, &AEnvironmentCanvasActor::UpdatePawn);

	Controller = PlayerController;
	Pawn = PlayerController->GetPawn();

	return true;
}

void AEnvironmentCanvasActor::DeinitializeController()
{
	APlayerController* PlayerController = Controller.Get();
	if (IsValid(PlayerController))
	{
		PlayerController->OnPossessedPawnChanged.RemoveAll(this);
	}

	Controller = nullptr;
	Pawn = nullptr;
}

void AEnvironmentCanvasActor::UpdatePawn(APawn* OldPawn, APawn* NewPawn)
{
	Pawn = NewPawn;
}


FVector2D AEnvironmentCanvasActor::MakePixelOffset() const
{
	APawn* Character = Pawn.Get();
	if (!IsValid(Character))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("Character is invalid"));
		return FVector2D::ZeroVector;
	}

	FVector CurrentLocation = GetActorLocation();
	FVector PlayerLocation = Character->GetActorLocation();

	float X = FMath::Floor(PlayerLocation.X / PixelRatio);
	float Y = FMath::Floor(PlayerLocation.Y / PixelRatio);

	FVector2D Snap2D = (FVector2D(X, Y) + 0.5f) * PixelRatio;
	FVector2D Location2D = FVector2D(CurrentLocation.X, CurrentLocation.Y);

	return (Snap2D - Location2D);
}

void AEnvironmentCanvasActor::MoveRenderTargets()
{
	if (!MPCInstance)
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("MPCInstance is invalid"));
		return;
	}

	PixelOffset = MakePixelOffset();

	AddActorWorldOffset(FVector(PixelOffset.X, PixelOffset.Y, 0.0f));

	MPCInstance->SetVectorParameterValue(EnvironmentPaintConstant::MPC_CanvasLocation, GetActorLocation());
}

void AEnvironmentCanvasActor::DrawRenderTargets()
{

}


#if WITH_EDITOR

void AEnvironmentCanvasActor::DrawDebug()
{
	UWorld* World = GetWorld();
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + FVector(0.0f, 0.0f, 100.0f);

	DrawDebugDirectionalArrow(World, StartLocation, EndLocation, 10.0f, FColor::Magenta, false, -1.0f, 0, 20.0f);
}

#endif


