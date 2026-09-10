// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Actor/RegionActor.h"

// Generated Headers
#include "WeatherRegionActor.generated.h"

// Forward Declarations
class UWeatherSubsystem;
class UWeatherCollectionAsset;
struct FStreamableHandle;


/**
 *
 */
UCLASS(Abstract)
class AWeatherRegionActor : public ARegionActor
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, meta = (AllowedTypes = "Weather.Collection"))
	FPrimaryAssetId WeatherCollectiontId;

	TObjectPtr<UWeatherSubsystem> WeatherSubsystem;
	TSharedPtr<FStreamableHandle> AssetHandle;


	void LoadWeather();
	void AddWeather();
	void RemoveWeather();

	const UWeatherCollectionAsset* GetWeatherWeight() const;

	// ~ Binding
	void HandleOnWeatherRefreshed();
	void HandleOnWeatherLoaded();
	// ~ End of Binding

	// ~ ARegionActor
	void HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	void HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex) override;
	// ~ End of ARegionActor

	// ~ AActor
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~ End of AActor

};

