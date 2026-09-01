// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Abilities/GameplayAbility.h"

// Generated Headers
#include "EquipmentAbility_Skill.generated.h"


/**
 *
 */
UCLASS(Abstract)
class UEquipmentAbility_Skill : public UGameplayAbility
{

	GENERATED_BODY()

public:

	// ~ UGameplayAbility
	virtual bool CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	// ~ End of UGameplayAbility

};

