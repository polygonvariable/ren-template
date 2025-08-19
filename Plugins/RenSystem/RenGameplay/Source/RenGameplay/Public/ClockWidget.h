// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

// Generated Headers
#include "ClockWidget.generated.h"

// Forward Declarations
class UTextBlock;
class UClockSubsystem;



/**
 *
 */
UCLASS(Abstract)
class UClockWidget : public UUserWidget
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Format = "mm:ss ap";

	UPROPERTY()
	TObjectPtr<UClockSubsystem> ClockSubsystem;

	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TimeTextBlock;


	void HandleTimeChanged(float Time);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};

