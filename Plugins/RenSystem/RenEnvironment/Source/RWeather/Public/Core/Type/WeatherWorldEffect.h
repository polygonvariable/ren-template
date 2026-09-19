// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "WeatherWorldEffect.generated.h"


/*
 *
 */
USTRUCT()
struct FWeatherWorldEffect
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FName ComponentTag;

	UPROPERTY(EditAnywhere, meta = (AllowedTypes = "Weather"))
	FPrimaryAssetId WeatherId;

	UPROPERTY(EditAnywhere)
	bool bCanTransition = true;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bCanTransition", ClampMin = "1.0", UIMin = "1.0"))
	float TransitionDuration = 5.0f;

	UPROPERTY(Transient, VisibleAnywhere)
	float CurrentAlpha = 0.0f;
	

	void Execute(UActorComponent* Component, bool bIsAdded);
	void SetAlpha(UActorComponent* Component, float NewAlpha);

	virtual ~FWeatherWorldEffect() = default;

protected:

	virtual void OnApplyChange(UActorComponent* Component, float OldAlpha, float NewAlpha) {};

};


/*
 *
 */
USTRUCT()
struct FWeatherWorldEffect_MoveComponent : public FWeatherWorldEffect
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FVector Offset = FVector::ZeroVector;

protected:

    // ~ FWeatherWorldEffect
	virtual void OnApplyChange(UActorComponent* Component, float OldAlpha, float NewAlpha) override;
    // ~ End of FWeatherWorldEffect

};


