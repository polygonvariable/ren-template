// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Abilities/GameplayAbility.h"

// Generated Headers
#include "EquipmentAbility.generated.h"


/**
 *
 */
UCLASS()
class UEquipmentAbility : public UGameplayAbility
{

	GENERATED_BODY()

public:

};


/**
 *
 */
UCLASS()
class UEquipmentSkillAbility : public UEquipmentAbility
{

	GENERATED_BODY()

public:

	// ~ UGameplayAbility
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual const FGameplayTagContainer* GetCooldownTags() const override;

	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	// ~ End of UGameplayAbility

protected:

	FGameplayTagContainer EquipmentCooldownTags;

};

