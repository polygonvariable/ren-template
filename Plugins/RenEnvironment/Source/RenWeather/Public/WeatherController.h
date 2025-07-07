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



/**
 * 
 */
UCLASS()
class UWeatherController : public UObjectPrioritySystem
{

	GENERATED_BODY()

public:

	void SetMaterialCollection(UMaterialParameterCollection* MaterialCollection);

	virtual void CleanUpItems() override;

protected:

	UPROPERTY()
	UMaterialParameterCollectionInstance* MaterialCollectionInstance;

	UPROPERTY()
	UWeatherAsset* CurrentWeatherAsset;

	FName CurrentWeatherName = NAME_None;

	void HandleScalarTransition(FName ParameterName, float Target, float Alpha);
	void HandleVectorTransition(FName ParameterName, const FLinearColor& Target, float Alpha);

protected:

	virtual void HandleItemChanged(UObject* Item) override;
	//virtual void HandleItemRemoved(UObject* Item) override;
	virtual void HandleNoItemsLeft() override;

};

