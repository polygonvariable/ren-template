// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "EnvironmentCanvasParameter.generated.h"


/*
 *
 */
USTRUCT(BlueprintType)
struct FEnvironmentCanvasParameter
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MPC_CanvasSize = TEXT("Canvas.Size");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MPC_CanvasLocation = TEXT("Canvas.Location");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NS_PixelRatio = TEXT("User.PixelRatio");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NS_PixelOffset = TEXT("User.PixelOffset");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NS_DrawPoints = TEXT("User.DrawPoints");

};

