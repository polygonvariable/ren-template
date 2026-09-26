// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Data/CoreDataAsset.h"

// Generated Headers
#include "InstanceableDataAsset.generated.h"


/**
 * A data asset that can be used to create a lightweight instance(struct) which can be stored
 * like in an inventory, characters, etc
 */
UCLASS(Abstract, MinimalAPI)
class UInstanceableDataAsset : public UCoreDataAsset
{

	GENERATED_BODY()

};

