// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "InstancedStruct.h"

// Project Headers
#include "Definition/CharacterInitializationData.h"

// Generated Headers
#include "EnemyData.generated.h"


/*
 - never spawn
 - spawn only once (wont be spawn after death, timestamp will be recorded)
 - respawn after delay (will be spawn again after delay when dead, timestamp will be recorded)
 - respawn immediately (will be spawn again after interaction, timestamp won't be recorded)
*/
UENUM(BlueprintType)
enum class EEnemyRespawnType : uint8
{
	Never UMETA(DisplayName = "Never Spawn"),
	SpawnOnce UMETA(DisplayName = "Spawn Once"),
	RespawnAfterDelay UMETA(DisplayName = "Respawn After Delay"),
	RespawnImmediately UMETA(DisplayName = "Respawn Immediately")
};

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

	UPROPERTY(EditAnywhere)
	EEnemyRespawnType RespawnType = EEnemyRespawnType::SpawnOnce;

	UPROPERTY(EditAnywhere, Meta = (EditConditionHides, EditCondition = "RespawnType==EEnemyRespawnType::RespawnAfterDelay"))
	FTimespan RespawnDelay = FTimespan::FromSeconds(60.0f);

	UPROPERTY(EditAnywhere)
	bool bEnableDrop = false;
	
	UPROPERTY(EditAnywhere, Meta = (EditConditionHides, EditCondition = "bEnableDrop==true"))
	FInstancedStruct DropData;

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
