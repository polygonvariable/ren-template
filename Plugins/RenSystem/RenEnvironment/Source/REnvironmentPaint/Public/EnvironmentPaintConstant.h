// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers
#include "EnvironmentPaintConstant.generated.h"

// Forward Declarations


USTRUCT(BlueprintType)
struct FEnvironmentCanvasParameters
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName A_BrushTag = TEXT("Environment.Brush");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MPC_CanvasSize = TEXT("Canvas.Size");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MPC_CanvasLocation = TEXT("Canvas.Location");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MPC_CanvasLocationScale = TEXT("Canvas.LocationScale");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName M_Offset = TEXT("Offset");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NS_PixelRatio = TEXT("User.PixelRatio");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NS_PixelOffset = TEXT("User.PixelOffset");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NS_DrawPoints = TEXT("User.DrawPoints");

};

