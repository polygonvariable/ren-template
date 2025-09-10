// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

// Generated Headers
#include "RenGameInstance.generated.h"



/**
 * 
 */
UCLASS()
class RCOREGAME_API URenGameInstance : public UGameInstance
{

	GENERATED_BODY()

public:

	virtual void Init() override;

};

