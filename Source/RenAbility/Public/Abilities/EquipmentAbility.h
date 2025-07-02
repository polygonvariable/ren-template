// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "Components/ActorComponent.h"
#include "InstancedStruct.h"
#include "NativeGameplayTags.h"

// Project Headers

// Generated Headers
#include "EquipmentAbility.generated.h"

// Forward Declarations
class URAbilitySystemComponent;



/**
 * 
 * 
 */
UCLASS()
class UEquipmentAbility : public UGameplayAbility
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> EquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> UnequipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StartTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BlendOut = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bStopAllMontages = false;

	FGameplayTag SlotTag;

	TWeakObjectPtr<URAbilitySystemComponent> RASC;

	void OnTagChanged(const FGameplayTag Tag, int32 NewCount);

public:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

};

