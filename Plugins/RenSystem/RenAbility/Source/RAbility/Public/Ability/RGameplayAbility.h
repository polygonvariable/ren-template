// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"

// Generated Headers
#include "RGameplayAbility.generated.h"


/**
 *
 */
UCLASS()
class URGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	FGameplayTagContainer GetDynamicTags(bool& bSuccess) const;

};

