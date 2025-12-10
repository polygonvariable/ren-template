// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EnvironmentCanvasActor.h"

// Engine Headers
#include "Kismet/KismetSystemLibrary.h"
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

#include "RenEnvironmentPaint/Public/EnvironmentBrushInterface.h"
#include "RenEnvironmentPaint/Public/EnvironmentBrushComponent.h"



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

	ResolvePendingBrushes();
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


void AEnvironmentCanvasActor::FindNearbyBrushes()
{
	FVector StartLocation = GetActorLocation();;

	TArray<TEnumAsByte<EObjectTypeQuery>> TypesArray;
	TypesArray.Reserve(3);
	TypesArray.Emplace(ECollisionChannel::ECC_Pawn);
	TypesArray.Emplace(ECollisionChannel::ECC_PhysicsBody);
	TypesArray.Emplace(ECollisionChannel::ECC_WorldDynamic);

	TArray<AActor*> IgnoredActors;
	TArray<FHitResult> OutHits;

	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), StartLocation, StartLocation, ScanDistance, TypesArray, false, IgnoredActors, EDrawDebugTrace::ForDuration, OutHits, true);
	if (bHit)
	{
		for (const FHitResult& Hit : OutHits)
		{
			RegisterBrush(Hit.GetActor());
		}
	}
}

void AEnvironmentCanvasActor::ClearBrushComponents()
{
	BrushCollection.Reset();
}

void AEnvironmentCanvasActor::GetBrushComponents(AActor* Actor, TArray<UActorComponent*>& OutComponents)
{
	OutComponents = Actor->GetComponentsByInterface(UEnvironmentBrushInterface::StaticClass());
}

void AEnvironmentCanvasActor::ResolvePendingBrushes()
{
	if (PendingBrushAdd.Num() > 0)
	{
		BrushCollection.Append(PendingBrushAdd);
		PendingBrushAdd.Reset();
	}

	if (PendingBrushRemove.Num() > 0)
	{
		BrushCollection.RemoveAll(
			[this](const TWeakInterfacePtr<IEnvironmentBrushInterface>& Interface)
			{
				return PendingBrushRemove.Contains(Interface);
			}
		);
		PendingBrushRemove.Reset();
	}
}

void AEnvironmentCanvasActor::StartBrushScan()
{
	if (BrushScanTimer.IsValid())
	{
		LOG_WARNING(LogEnvironment, TEXT("BrushScanTimer is already valid"));
		return;
	}

	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();

	TimerManager.SetTimer(BrushScanTimer, this, &AEnvironmentCanvasActor::FindNearbyBrushes, ScanInterval, true);
}

void AEnvironmentCanvasActor::StopBrushScan()
{
	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();

	TimerManager.ClearTimer(BrushScanTimer);

	BrushScanTimer.Invalidate();
}

void AEnvironmentCanvasActor::RegisterBrush(AActor* Actor)
{
	int Num = BrushCollection.Num();
	if (MaxBrushes > 0 && Num >= MaxBrushes)
	{
		LOG_WARNING(LogEnvironment, TEXT("Max brushes reached"));
		return;
	}

	if (!IsValid(Actor) || !Actor->ActorHasTag(CanvasParameters.A_BrushTag))
	{
		LOG_ERROR(LogEnvironment, TEXT("Actor is invalid or not a brush"));
		return;
	}

	TArray<UActorComponent*> Components;
	GetBrushComponents(Actor, Components);

	for (UActorComponent* Component : Components)
	{
		IEnvironmentBrushInterface* Interface = Cast<IEnvironmentBrushInterface>(Component);
		if (Interface && !BrushCollection.Contains(Interface))
		{
			PendingBrushAdd.Add(TWeakInterfacePtr<IEnvironmentBrushInterface>(Interface));
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

	FindNearbyBrushes();
	StartBrushScan();

	SetActorTickEnabled(true);

	return true;
}

void AEnvironmentCanvasActor::Deinitialize()
{
	SetActorTickEnabled(false);

	StopBrushScan();
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

	MPCInstance->SetScalarParameterValue(CanvasParameters.MPC_CanvasSize, CanvasSize);
	MPCInstance->SetVectorParameterValue(CanvasParameters.MPC_CanvasLocation, GetActorLocation());

	return true;
}

bool AEnvironmentCanvasActor::InitializePixel()
{
	if (CanvasSize <= 0.0f || ImageSize <= 0.0f)
	{
		LOG_ERROR(LogEnvironment, TEXT("CanvasSize or ImageSize is invalid"));
		return false;
	}

	PixelRatio = (1.0 / ImageSize) * CanvasSize;

	return true;
}

bool AEnvironmentCanvasActor::InitializeRenderTargets()
{
	if (!MainRenderTarget || !PersistentRenderTarget)
	{
		LOG_ERROR(LogEnvironment, TEXT("MainRT, PersistentRT is invalid"));
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

	MPCInstance->SetVectorParameterValue(CanvasParameters.MPC_CanvasLocation, GetActorLocation());
}

void AEnvironmentCanvasActor::DrawRenderTargets()
{

}

float AEnvironmentCanvasActor::PackFloats(float ValueA, float ValueB)
{
	FFloat16 A(ValueA);
	FFloat16 B(ValueB);

	uint32 PackedInt = (uint32(A.Encoded) << 16) | uint32(B.Encoded);

	float PackedFloat;
	FMemory::Memcpy(&PackedFloat, &PackedInt, sizeof(float));

	return PackedFloat;
}

#if WITH_EDITOR

void AEnvironmentCanvasActor::DrawDebug()
{
	if (!bDrawDebug)
	{
		return;
	}

	UWorld* World = GetWorld();
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + FVector(0.0f, 0.0f, 100.0f);

	DrawDebugDirectionalArrow(World, StartLocation, EndLocation, 10.0f, FColor::Magenta, false, -1.0f, 0, 20.0f);
}

#endif

