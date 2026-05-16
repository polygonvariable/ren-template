// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/CharacterInitializationData.h"

// Generated Headers
#include "EnemyData.generated.h"


/*
 *
 */
USTRUCT(BlueprintType)
struct FEnemyInitializationData
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FGuid EnemyId;

	UPROPERTY(EditAnywhere)
	FTransform SpawnTransform;

};

/*
 *
 */
USTRUCT()
struct FEnemySpawnData
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FCharacterInitializationData CharacterData;

	UPROPERTY(EditAnywhere)
	FEnemyInitializationData EnemyData;

};
