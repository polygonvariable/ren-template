// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Abilities/GameplayAbility.h"

// Generated Headers
#include "CharacterDeadAbility.generated.h"


/**
 * 
 */
UCLASS()
class UCharacterDeadAbility : public UGameplayAbility
{

	GENERATED_BODY()
	
public:

	UCharacterDeadAbility();

	// ~ UGameplayAbility
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// ~ End of UGameplayAbility

protected:

	UPROPERTY(VisibleAnywhere)
	FGameplayTag DeadStateTag;

	FTransform MeshTransform;


	void HandleOnTagChanged(const FGameplayTag Tag, int Count);

};

