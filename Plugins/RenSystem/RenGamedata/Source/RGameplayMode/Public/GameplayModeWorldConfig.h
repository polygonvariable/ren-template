// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers
#include "WorldConfigAsset.h"

// Generated Headers
#include "GameplayModeWorldConfig.generated.h"


/**
 *
 */
UCLASS(MinimalAPI)
class UGameplayModeWorldConfig : public UWorldConfigAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Mode")
	bool bEnabled = true;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (Categories = "Gameplay"))
	FGameplayTagContainer DefaultMode;


#if WITH_EDITOR
	// ~ UPrimaryDataAsset
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
	// ~ End of UPrimaryDataAsset
#endif

};

