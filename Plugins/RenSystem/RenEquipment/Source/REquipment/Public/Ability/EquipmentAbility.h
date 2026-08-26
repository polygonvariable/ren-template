// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Abilities/GameplayAbility.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

// Generated Headers
#include "EquipmentAbility.generated.h"

// Forward Declarations
class UEquipmentController;
class AEquipmentActor;


/**
 *
 */
UCLASS(Abstract)
class UEquipmentAbility : public UGameplayAbility
{

	GENERATED_BODY()

public:

};




/**
 *
 */
UCLASS(Abstract)
class UEquipmentStateAbility : public UEquipmentAbility
{

	GENERATED_BODY()

public:

	// ~ UGameplayAbility
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	// ~ End of UGameplayAbility

#if WITH_EDITOR
	// ~ UObject
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
	// ~ End of UObject
#endif

protected:

	UFUNCTION(BlueprintCallable)
	void EnableEquipment();

	UFUNCTION(BlueprintCallable)
	void DisableEquipment();

	UFUNCTION(BlueprintCallable)
	UEquipmentController* GetController() const;

};


/**
 *
 */
UCLASS(Abstract)
class UEquipmentWeaponAbility : public UEquipmentAbility
{

	GENERATED_BODY()

public:

	// ~ UGameplayAbility
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	// ~ End of UGameplayAbility

#if WITH_EDITOR
	// ~ UObject
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
	// ~ End of UObject
#endif

protected:

	UFUNCTION(BlueprintCallable)
	UEquipmentController* GetEquipmentController() const;

	UFUNCTION(BlueprintCallable)
	AEquipmentActor* GetEquipmentActor() const;

};






/**
 *
 */
UCLASS(Abstract)
class UEquipmentSkillAbility : public UEquipmentAbility
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

