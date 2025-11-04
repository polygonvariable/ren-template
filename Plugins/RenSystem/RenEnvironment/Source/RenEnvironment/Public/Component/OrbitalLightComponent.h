// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Components/DirectionalLightComponent.h"

// Generated Headers
#include "OrbitalLightComponent.generated.h"



/**
 * 
 */
UCLASS(ClassGroup = (Custom), Meta = (BlueprintSpawnableComponent))
class UOrbitalLightComponent : public UDirectionalLightComponent
{

	GENERATED_BODY()

public:

	UOrbitalLightComponent();


	UPROPERTY(EditAnywhere)
	bool bInverseRotation = false;


	UPROPERTY(EditAnywhere, Meta = (UIMin = "1800", UIMax = "2200", ClampMax = "4000", ClampMin = "0"))
	int Year = 2022;

	UPROPERTY(EditAnywhere, Meta = (UIMin = "1", ClampMin = "1", UIMax = "12", ClampMax = "12"))
	int Month = 9;

	UPROPERTY(EditAnywhere, Meta = (UIMin = "1", ClampMin = "1", UIMax = "31", ClampMax = "31"))
	int Day = 21;


	UPROPERTY(EditAnywhere, Meta = (UIMin = "-89.99", ClampMin = "-89.99", UIMax = "89.99", ClampMax = "89.99"))
	float Latitude = 45.0f;

	UPROPERTY(EditAnywhere, Meta = (UIMax = "180", UIMin = "-180", ClampMin = "-180", ClampMax = "180"))
	float Longitude = -73.0f;

	UPROPERTY(EditAnywhere, Meta = (UIMin = "-12", ClampMin = "-12", UIMax = "14", ClampMax = "14"))
	float TimeZone = -4.0f;

	UPROPERTY(EditAnywhere, Meta = (UIMin = "-360", ClampMin = "-360", UIMax = "360", ClampMax = "360"))
	float NorthPoleOffset = 0.0f;


	UFUNCTION(BlueprintCallable)
	void SetTimeOfDay(float NewTime);

	UFUNCTION(BlueprintCallable)
	float GetTimeOfDay();

protected:

	UPROPERTY(EditAnywhere, Meta = (UIMin = "0", UIMax = "24", ClampMin = "0", ClampMax = "24"))
	float TimeOfDay = 12.0f;

};

