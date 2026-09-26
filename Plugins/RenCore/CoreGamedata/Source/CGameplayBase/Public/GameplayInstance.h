// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/GameInstance.h"

// Generated Headers
#include "GameplayInstance.generated.h"


/**
 * 
 */
UCLASS(MinimalAPI)
class UGameplayInstance : public UGameInstance
{

	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE(FOnGameInstanceSubsystemsInitialized);
	FOnGameInstanceSubsystemsInitialized OnGameInstanceSubsystemsInitialized;


	// ~ UGameplayInstance
	virtual void Init() override;
	// ~ End of UGameplayInstance

protected:

	virtual void GameInit();

};

