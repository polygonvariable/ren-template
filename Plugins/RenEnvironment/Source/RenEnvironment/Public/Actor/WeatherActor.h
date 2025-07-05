// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Project Headers
#include "RenEnvironment/Public/Profile/WeatherProfile.h"

// Generated Headers
#include "WeatherActor.generated.h"

// Forward Declarations
class UTimer;
class UPrioritySystem;
class UMaterialParameterCollectionInstance;
class UWeatherController;
class UWeatherSubsystem;
class UWeatherAsset;


/**
 *
 */
UCLASS()
class AWeatherActor : public AActor
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UWeatherAsset> WeatherAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Priority;


	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UWeatherSubsystem> WeatherSubsystem;


	/**
	* TMap of weights to weather profiles
	* TMap<int = Weight, FWeatherProfile = Weather Profile>
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<int, FString> WeatherCollection;



	UFUNCTION(BlueprintCallable, Meta = (BlueprintProtected))
	void AddWeather();

	UFUNCTION(BlueprintCallable, Meta = (BlueprintProtected))
	void RemoveWeather();



	UFUNCTION()
	void HandleWeatherCanChange();



	UFUNCTION(BlueprintCallable)
	FString Roll(const TMap<int, FString>& Items);

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};

