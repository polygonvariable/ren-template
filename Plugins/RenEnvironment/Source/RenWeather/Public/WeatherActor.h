// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Project Headers
#include "RenCore/Public/Actor/RegionActor.h"

// Generated Headers
#include "WeatherActor.generated.h"

// Forward Declarations
class UWeatherSubsystem;
class UWeatherAsset;



/**
 *
 */
UCLASS(Abstract)
class AWeatherRegionActor : public ARegionActor
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UWeatherAsset>> WeatherAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int WeatherPriority = 20;
	int CurrentIndex = 0;

	UPROPERTY()
	TWeakObjectPtr<UWeatherSubsystem> WeatherSubsystem;

	UPROPERTY()
	TObjectPtr<UWeatherAsset> CurrentWeatherAsset;


	void AddWeather();
	void RemoveWeather();
	void HandleWeatherRefresh();

protected:

	virtual void HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex) override;

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};

