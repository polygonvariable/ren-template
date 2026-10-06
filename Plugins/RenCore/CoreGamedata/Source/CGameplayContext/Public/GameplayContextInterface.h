// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "StructUtils/InstancedStruct.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"

// Project Headers
#include "GameplayContextAction.h"

// Generated Headers
#include "GameplayContextInterface.generated.h"


UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UGameplayContextInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class CGAMEPLAYCONTEXT_API IGameplayContextInterface
{

	GENERATED_BODY()

public:

	virtual void PushContext(TInstancedStruct<FGameplayContextAction>&& Context) = 0;
	virtual void Execute(const FGameplayTagContainer& ContextTags, UObject* Caller) = 0;

};

