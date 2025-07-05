// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "InstancedStruct.h"

// Project Headers
#include "RenEnvironment/Public/Profile/WeatherProfile.h"

// Generated Headers
#include "WeatherSubsystem.generated.h"

// Forward Declarations
class UTimer;
class UWeatherController;
class UEnvironmentAsset;



/**
 *
 */
UCLASS(Blueprintable)
class UWeatherSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void AddWeather(UWeatherAsset* WeatherAsset, int Priority);

	UFUNCTION(BlueprintCallable)
	void RemoveWeather(int Priority);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float WeatherChangeTime = 10.0f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTimer> WeatherTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UWeatherController> WeatherController;



	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UEnvironmentAsset> EnvironmentAsset;



	UFUNCTION()
	void CreateWeatherTimer();

	UFUNCTION()
	void HandleWeatherTimer(float ElapsedTime);



	UFUNCTION()
	bool CreateWeatherController();

	UFUNCTION()
	void CreateWeatherMaterialCollection();


protected:

	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void PostInitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeatherCanChange);
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnWeatherCanChange OnWeatherCanChange;

};

