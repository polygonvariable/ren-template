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
#include "RenEnvironmentFx/Public/EnvironmentPaintConstant.h"




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

bool AEnvironmentNiagaraCanvas::InitializePixel()
{
	if (!Super::InitializePixel())
	{
		return false;
	}

	NiagaraComponent->SetVariableFloat(EnvironmentPaintConstant::NS_PixelRatio, PixelRatio);

	return true;
}

void AEnvironmentNiagaraCanvas::MoveRenderTargets()
{
	if (!MPCInstance)
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("MPCInstance is invalid"));
		return;
	}

	PixelOffset = MakePixelOffset();

	NiagaraComponent->SetVariableVec2(EnvironmentPaintConstant::NS_PixelOffset, (PixelOffset / CanvasSize));

	AddActorWorldOffset(FVector(PixelOffset.X, PixelOffset.Y, 0.0f));

	MPCInstance->SetVectorParameterValue(EnvironmentPaintConstant::MPC_CanvasLocation, GetActorLocation());

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
	
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector4(NiagaraComponent, EnvironmentPaintConstant::NS_DrawPoints, BrushPoints);
}

