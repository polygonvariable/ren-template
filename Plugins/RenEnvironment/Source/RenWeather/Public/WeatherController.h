// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenCore/Public/Priority/PrioritySystem.h"

// Generated Headers
#include "WeatherController.generated.h"

// Forward Declarations
class UMaterialParameterCollectionInstance;
class UWeatherAsset;

// Delegate Declarations
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeatherChanged, UWeatherAsset* /* WeatherAsset */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeatherRemoved, UWeatherAsset* /* WeatherAsset */);



/**
 * 
 */
UCLASS()
class UWeatherController : public UObjectPrioritySystem
{

	GENERATED_BODY()

public:

	void SetMaterialCollection(UMaterialParameterCollection* MaterialCollection);

protected:

	UPROPERTY()
	UMaterialParameterCollectionInstance* MaterialCollectionInstance;

	UPROPERTY()
	TObjectPtr<UWeatherAsset> CurrentWeatherAsset;


	void HandleScalarTransition(FName ParameterName, float Target, float Alpha);
	void HandleVectorTransition(FName ParameterName, const FLinearColor& Target, float Alpha);

public:

	virtual void CleanUpItems() override;

protected:

	virtual void HandleItemChanged(UObject* Item) override;
	virtual void HandleItemRemoved(UObject* Item, bool bWasReplaced) override;
	virtual void HandleNoItemsLeft() override;

public:

	FOnWeatherChanged OnWeatherChanged;
	FOnWeatherRemoved OnWeatherRemoved;

};

