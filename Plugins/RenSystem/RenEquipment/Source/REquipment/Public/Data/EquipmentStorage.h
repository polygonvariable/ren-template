// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/SaveGame.h"

// Project Headers
#include "Core/Type/Runtime/EquipmentInstance.h"

// Generated Headers
#include "EquipmentStorage.generated.h"


/**
 *
 */
UCLASS(MinimalAPI)
class UEquipmentStorage : public USaveGame
{

	GENERATED_BODY()

public:

	UPROPERTY(SaveGame)
	TMap<FGuid, FEquipmentOwnerInstance> EquipmentOwners;

	UPROPERTY(SaveGame)
	TMap<FGuid, FEquipmentSlotInstance> EquipmentInstances;

};

