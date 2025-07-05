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
class RENENVIRONMENT_API UOrbitalLightComponent : public UDirectionalLightComponent
{

	GENERATED_BODY()

public:

	UOrbitalLightComponent();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbital Light|Time")
	bool bInverseRotation = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (UIMin = "1800", UIMax = "2200", ClampMax = "4000", ClampMin = "0"), Category = "Orbital Light|Date")
	int Year = 2022;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (UIMin = "1", ClampMin = "1", UIMax = "12", ClampMax = "12"), Category = "Orbital Light|Date")
	int Month = 9;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (UIMin = "1", ClampMin = "1", UIMax = "31", ClampMax = "31"), Category = "Orbital Light|Date")
	int Day = 21;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (UIMin = "-89.99", ClampMin = "-89.99", UIMax = "89.99", ClampMax = "89.99"), Category = "Orbital Light|Location")
	float Latitude = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (UIMax = "180", UIMin = "-180", ClampMin = "-180", ClampMax = "180"), Category = "Orbital Light|Location")
	float Longitude = -73.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (UIMin = "-12", ClampMin = "-12", UIMax = "14", ClampMax = "14"), Category = "Orbital Light|Location")
	float TimeZone = -4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (UIMin = "-360", ClampMin = "-360", UIMax = "360", ClampMax = "360"), Category = "Orbital Light|Location")
	float NorthPoleOffset = 0.0f;


	UFUNCTION(BlueprintCallable)
	void SetTime(float NewTime);

	UFUNCTION(BlueprintCallable)
	float GetTime();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (UIMin = "0", UIMax = "24", ClampMin = "0", ClampMax = "24"), Category = "Orbital Light|Time")
	float Time = 12.0f;

};

