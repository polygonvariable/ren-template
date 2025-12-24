// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "Abilities/GameplayAbility.h"

// Generated Headers
#include "EntitySprintAbility.generated.h"

// Forward Declarations
class UCharacterMovementComponent;



/**
 * 
 * 
 * 
 */
UCLASS(Abstract)
class UEntitySprintAbility : public UGameplayAbility
{

	GENERATED_BODY()
	
public:

	// ~ UGameplayAbility
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// ~ End of UGameplayAbility

protected:

	UPROPERTY(EditAnywhere)
	FGameplayAttribute StaminaAttribute;

	UPROPERTY(EditAnywhere)
	FGameplayTag SprintTag;


	UPROPERTY(EditAnywhere)
	float SprintSpeed = 500.0f;

	UPROPERTY(EditAnywhere)
	float SprintMinSpeed = 50.0f;


	float PreviousSpeed = 100.0f;


	UCharacterMovementComponent* GetCharacterMovement() const;
	
	void OnStaminaChanged(const FOnAttributeChangeData& Data);

};

