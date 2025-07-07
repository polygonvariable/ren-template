// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Project Headers
#include "RenCore/Public/Actor/RegionActor.h"

#include "RenWeather/Public/WeatherProfile.h"

// Generated Headers
#include "WeatherActor.generated.h"

// Forward Declarations
class UWeatherSubsystem;
class UWeatherAsset;


/**
 *
 */
UCLASS()
class AWeatherRegionActor : public ARegionActor
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UWeatherAsset> WeatherAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Priority = 20;


	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UWeatherSubsystem> WeatherSubsystem;


	void AddWeather();
	void RemoveWeather();


	virtual void HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex) override;


	UFUNCTION()
	void HandleWeatherCanChange();

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};

