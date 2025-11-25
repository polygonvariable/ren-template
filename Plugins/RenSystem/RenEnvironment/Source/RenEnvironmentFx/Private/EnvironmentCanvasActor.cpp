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




AEnvironmentCanvasActor::AEnvironmentCanvasActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AEnvironmentCanvasActor::FindBrushComponents()
{
	UWorld* World = GetWorld();
	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		RegisterBrush(*ActorItr);
	}
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

	TArray<UActorComponent*> Components = Actor->GetComponentsByInterface(UEnvironmentBrushInterface::StaticClass());

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

}


void AEnvironmentCanvasActor::BeginPlay()
{
	Super::BeginPlay();

	if (!MPC || !MainRenderTarget || !PersistentRenderTarget || !DrawBrushMaterial || !DrawMainAdditiveMaterial)
	{
		LOG_ERROR(LogEnvironment, TEXT("Invalid parameters"));
		return;
	}

	UWorld* World = GetWorld();
	MPCInstance = World->GetParameterCollectionInstance(MPC);

	DrawPersistentMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, DrawPersistentMaterial);

	FLinearColor ClearColor = FLinearColor::Black;
	UKismetRenderingLibrary::ClearRenderTarget2D(this, MainRenderTarget, ClearColor);
	UKismetRenderingLibrary::ClearRenderTarget2D(this, PersistentRenderTarget, ClearColor);

	if (!MPCInstance || !DrawPersistentMaterialInstance)
	{
		LOG_ERROR(LogEnvironment, TEXT("Invalid runtime parameters"));
		return;
	}

	FindBrushComponents();

	MPCInstance->SetScalarParameterValue(TEXT("Size"), CanvasSize);
	PixelRatio = (1.0f / ImageSize) * CanvasSize;

	SetActorTickEnabled(true);
}

void AEnvironmentCanvasActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveRenderTargets();
	DrawPersistentRenderTarget();
	DrawMainRenderTarget();
}
void AEnvironmentCanvasActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SetActorTickEnabled(false);

	Super::EndPlay(EndPlayReason);
}


void AEnvironmentCanvasActor::MoveRenderTargets()
{
	if (!MPCInstance)
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("MPCInstance is invalid"));
		return;
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!IsValid(PlayerController))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("PlayerController is invalid"));
		return;
	}

	ACharacter* PlayerCharacter = PlayerController->GetCharacter();
	if (!IsValid(PlayerCharacter))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("PlayerCharacter is invalid"));
		return;
	}

	FVector CurrentLocation = GetActorLocation();
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	
	float X = FMath::Floor(PlayerLocation.X / PixelRatio);
	float Y = FMath::Floor(PlayerLocation.Y / PixelRatio);

	FVector2D Snap2D = (FVector2D(X, Y) + 0.5f) * PixelRatio;
	FVector2D Location2D = FVector2D(CurrentLocation.X, CurrentLocation.Y);

	PixelOffset = Snap2D - Location2D;

	AddActorWorldOffset(FVector(PixelOffset.X, PixelOffset.Y, 0.0f));

	MPCInstance->SetVectorParameterValue(TEXT("Loc"), GetActorLocation());
}

void AEnvironmentCanvasActor::DrawPersistentRenderTarget()
{
	if (!PersistentRenderTarget || !DrawPersistentMaterialInstance)
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("PersistentRenderTarget, DrawPersistentMaterialInstance is invalid"));
		return;
	}

	UCanvas* Canvas;
	FVector2D Size;
	FDrawToRenderTargetContext Context;
	 
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, PersistentRenderTarget, Canvas, Size, Context);

	FVector2D Ratio = PixelOffset / CanvasSize;
	FVector4 Offset = FVector4(Ratio.X, Ratio.Y, 0.0f, 1.0f);

	DrawPersistentMaterialInstance->SetVectorParameterValue(TEXT("Offset"), Offset);

	Canvas->K2_DrawMaterial(DrawPersistentMaterialInstance, FVector2D(0.0f, 0.0f), Size, FVector2D(0.0f, 0.0f), FVector2D(1.0f, 1.0f));
	
	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, Context);
}

void AEnvironmentCanvasActor::DrawMainRenderTarget()
{
	if (!MainRenderTarget || !DrawBrushMaterial || !DrawMainAdditiveMaterial)
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("MainRenderTarget, DrawBrushMaterial, DrawMainAdditiveMaterial is invalid"));
		return;
	}

	FVector CurrentLocation = GetActorLocation();

	UCanvas* Canvas;
	FVector2D Size;
	FDrawToRenderTargetContext Context;

	UKismetRenderingLibrary::ClearRenderTarget2D(this, MainRenderTarget, FLinearColor::Black);
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, MainRenderTarget, Canvas, Size, Context);

	FVector2D SizeHalf = Size / 2;

	for (TWeakInterfacePtr<IEnvironmentBrushInterface> Interface : BrushCollection)
	{
		IEnvironmentBrushInterface* Brush = Interface.Get();
		if (!Brush)
		{
			continue;
		}

		FVector ComponentLocation;
		FVector2D BrushSize = FVector2D(4);

		bool bCanDraw = Brush->GetBrushDetails(ComponentLocation, BrushSize);
		if (!bCanDraw)
		{
			continue;
		}

		FVector2D BrushSizeHalf = BrushSize / 2;

		float DX = (ComponentLocation.X - CurrentLocation.X) / PixelRatio;
		float DY = (ComponentLocation.Y - CurrentLocation.Y) / PixelRatio;

		FVector2D DrawPosition = (FVector2D(DX, DY) + SizeHalf) - BrushSizeHalf;

		Canvas->K2_DrawMaterial(DrawBrushMaterial, DrawPosition, BrushSize, FVector2D(0.0f, 0.0f), FVector2D(1.0f, 1.0f));
	}

	Canvas->K2_DrawMaterial(DrawMainAdditiveMaterial, FVector2D(0.0f, 0.0f), Size, FVector2D(0.0f, 0.0f), FVector2D(1.0f, 1.0f));

	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, Context);
}

