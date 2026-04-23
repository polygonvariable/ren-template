// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "EquipmentTagData.generated.h"


/**
 *
 */
USTRUCT(BlueprintType)
struct FEquipmentTagData
{

	GENERATED_BODY()

public:

	/*
	 * Dynamic tag granted to ability, used to identify ability for activation.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AbilityTag;

	/*
	 * Tag that responds to event, can be used to activate ability.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EventTag;

	/*
	 * Tag granted to actor, can be used to identify state of ability or actor.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag StateTag;

	/*
	 * 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag CooldownTag;

	/*
	 * Tag granted to actor, can be used to identify state of ability or actor.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ComponentName;

};

