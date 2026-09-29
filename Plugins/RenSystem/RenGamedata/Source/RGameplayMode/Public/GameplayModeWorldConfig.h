// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

	UPROPERTY(EditDefaultsOnly, Category = "Default", Meta = (GetOptions = "GameplayModeSettings.GetGameplayModeTableRows"))
	FName DefaultMode;


#if WITH_EDITOR
	// ~ UPrimaryDataAsset
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
	// ~ End of UPrimaryDataAsset
#endif

};

