// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/GameInstance.h"

// Generated Headers
#include "RGameInstance.generated.h"


/**
 * 
 */
UCLASS(MinimalAPI)
class URGameInstance : public UGameInstance
{

	GENERATED_BODY()

public:

	// ~ URGameInstance
	virtual void Init() override;
	// ~ End of URGameInstance

protected:

	virtual void GameInit();

};

