// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Actor/CharacterBase.h"
#include "Core/Type/EnemyData.h"

// Generated Headers
#include "EnemyCharacter.generated.h"


/**
 * 
 */
UCLASS(Abstract, MinimalAPI)
class AEnemyCharacter : public ACharacterBase
{

	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, AdvancedDisplay)
	FEnemyInitializationData EnemyData;

};

