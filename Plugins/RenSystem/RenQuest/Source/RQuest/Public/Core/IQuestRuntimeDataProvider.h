// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Generated Headers
#include "IQuestRuntimeDataProvider.generated.h"


/**
 *
 */
USTRUCT()
struct FQuestRuntimeData
{

	GENERATED_BODY()

public:

	FString Value;

};


UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UQuestRuntimeDataProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class IQuestRuntimeDataProvider
{

	GENERATED_BODY()

public:

	virtual void PullQuestData(TInstancedStruct<FQuestRuntimeData>& OutData) {};

};

