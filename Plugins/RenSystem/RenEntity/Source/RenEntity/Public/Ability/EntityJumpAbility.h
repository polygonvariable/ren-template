// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "Abilities/GameplayAbility.h"

// Generated Headers
#include "EntityJumpAbility.generated.h"

// Forward Declarations



/**
 * 
 * 
 * 
 */
UCLASS(Abstract)
class UEntityJumpAbility : public UGameplayAbility
{

	GENERATED_BODY()
	
public:

	// ~ UGameplayAbility
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	// ~ End of UGameplayAbility

};

