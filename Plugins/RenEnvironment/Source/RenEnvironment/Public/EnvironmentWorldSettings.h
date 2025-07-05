// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"

// Generated Headers
#include "EnvironmentWorldSettings.generated.h"

// Forward Declarations
class UEnvironmentAsset;



/**
 * 
 */
UCLASS()
class AEnvironmentWorldSettings : public AWorldSettings
{

	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UEnvironmentAsset> EnvironmentAsset;

};
