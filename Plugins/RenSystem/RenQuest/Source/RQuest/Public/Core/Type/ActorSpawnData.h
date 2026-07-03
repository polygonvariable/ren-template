// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "PropertyBag.h"

// Generated Headers
#include "ActorSpawnData.generated.h"

// Forward Declarations
class AQuestObjectiveActor;


/**
 *
 */
USTRUCT(BlueprintType)
struct FActorSpawnData
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AQuestObjectiveActor> ActorClass;

	UPROPERTY(EditAnywhere)
	FTransform Transform;

	UPROPERTY(EditAnywhere)
	FInstancedPropertyBag Properties;

};

