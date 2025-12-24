// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"

// Generated Headers
#include "Entity.generated.h"



/**
 * 
 * 
 * 
 */
UCLASS(Abstract, MinimalAPI)
class AEntity : public ACharacter
{

	GENERATED_BODY()

public:

	AEntity();

	// ~ AEntity
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// ~ End of AEntity

};

