// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "EnemyPayload.generated.h"


/**
 *
 */
USTRUCT()
struct FEnemyPartyPayload
{

	GENERATED_BODY()

public:

	FEnemyPartyPayload() {}
	FEnemyPartyPayload(int Total, int Remaining) : TotalEnemies(Total), RemainingEnemies(Remaining) {}

	int TotalEnemies = 0;
	int RemainingEnemies = 0;

};

