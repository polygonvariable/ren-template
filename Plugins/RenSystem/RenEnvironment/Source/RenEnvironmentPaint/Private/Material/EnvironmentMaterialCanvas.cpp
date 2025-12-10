// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Material/EnvironmentMaterialCanvas.h"

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

#include "RenEnvironmentPaint/Public/EnvironmentBrushInterface.h"
#include "RenEnvironmentPaint/Public/EnvironmentBrushComponent.h"



bool AEnvironmentMaterialCanvas::InitializeRenderTargets()
{
	if (!Super::InitializeRenderTargets())
	{
		return false;
	}

	if (!DrawPersistentMaterial)
	{
		LOG_ERROR(LogEnvironment, TEXT("PersistentMaterial, MainRT, PersistentRT is invalid"));
		return false;
	}

	DrawPersistentMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, DrawPersistentMaterial);
	if (!DrawPersistentMaterialInstance)
	{
		LOG_ERROR(LogEnvironment, TEXT("Failed to create DrawPersistentMaterialInstance"));
		return false;
	}

	return true;
}

void AEnvironmentMaterialCanvas::DrawRenderTargets()
{
	DrawPersistentRenderTarget();
	DrawMainRenderTarget();
}

void AEnvironmentMaterialCanvas::DrawPersistentRenderTarget()
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

	DrawPersistentMaterialInstance->SetVectorParameterValue(CanvasParameters.M_Offset, Offset);

	Canvas->K2_DrawMaterial(DrawPersistentMaterialInstance, FVector2D(0.0f, 0.0f), Size, FVector2D(0.0f, 0.0f), FVector2D(1.0f, 1.0f));

	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, Context);
}

void AEnvironmentMaterialCanvas::DrawMainRenderTarget()
{
	if (!MainRenderTarget || !DrawBrushMaterial)
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("MainRenderTarget, DrawBrushMaterial is invalid"));
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

		FVector Location = FVector::ZeroVector;
		FVector Velocity = FVector::ZeroVector;
		FVector2D BrushSize = FVector2D(4);
		float Density = 1.0f;

		bool bCanDraw = Brush->GetBrushDetails(Location, Velocity, BrushSize, Density);
		if (!bCanDraw)
		{
			continue;
		}

		FVector2D BrushSizeHalf = BrushSize / 2;

		float DX = (Location.X - CurrentLocation.X) / PixelRatio;
		float DY = (Location.Y - CurrentLocation.Y) / PixelRatio;

		FVector2D DrawPosition = (FVector2D(DX, DY) + SizeHalf) - BrushSizeHalf;

		Canvas->K2_DrawMaterial(DrawBrushMaterial, DrawPosition, BrushSize, FVector2D(0.0f, 0.0f), FVector2D(1.0f, 1.0f));
	}

	Canvas->K2_DrawTexture(PersistentRenderTarget, FVector2D(0.0f, 0.0f), Size, FVector2D(0.0f, 0.0f), FVector2D(1.0f, 1.0f), FLinearColor::White, EBlendMode::BLEND_Additive);

	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, Context);
}

