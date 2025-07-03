// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

// Generated Headers
#include "GameClockWidget.generated.h"

// Forward Declarations
class UTextBlock;
class UGameClockSubsystem;


/**
 *
 */
UCLASS(Abstract)
class RENGAMEPLAY_API UGameClockWidget : public UUserWidget
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Format = "mm:ss ap";

	UPROPERTY()
	TObjectPtr<UGameClockSubsystem> GameClockSubsystem;

	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TimeTextBlock;


	UFUNCTION()
	void HandleClockTick(float Time);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};