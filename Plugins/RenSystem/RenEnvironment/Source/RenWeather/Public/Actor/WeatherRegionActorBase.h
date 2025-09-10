// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RCoreCommon/Public/Actor/RegionActor.h"

// Generated Headers
#include "WeatherRegionActorBase.generated.h"

// Forward Declarations
class UWeatherSubsystem;
class UWeatherAsset;



/**
 *
 */
UCLASS(Abstract)
class AWeatherRegionActorBase : public ARegionActor
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int WeatherPriority = 20;

	UPROPERTY()
	TObjectPtr<UWeatherAsset> CurrentWeather;

	UPROPERTY()
	TWeakObjectPtr<UWeatherSubsystem> WeatherSubsystem;


	virtual void AddWeather();
	virtual void RemoveWeather();
	virtual void HandleWeatherRefreshed();

protected:

	virtual void HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex) override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};

