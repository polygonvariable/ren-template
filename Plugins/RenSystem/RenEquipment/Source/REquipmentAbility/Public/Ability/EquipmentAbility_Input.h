// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Abilities/GameplayAbility.h"

// Generated Headers
#include "EquipmentAbility_Input.generated.h"

// Module Macro
#define REN_API REQUIPMENTABILITY_API

// Forward Declaration
class UAnimMontage;
class UAbilityTask_WaitDelay;
class UAbilityTask_WaitInputRelease;


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UEquipmentAbility_Input : public UGameplayAbility
{

	GENERATED_BODY()

public:

    REN_API UEquipmentAbility_Input();

    // ~ UGameplayAbility
    REN_API virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    REN_API virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
    // ~ End of UGameplayAbility

protected:

    UPROPERTY(EditAnywhere)
    float HoldDelay = 0.45f;
    

    REN_API void RegisterInput();
    REN_API void UnregisterInput();

    REN_API virtual void OnInputSingle();
    REN_API virtual void OnInputHold();

    // ~ Binding
    UFUNCTION()
    void HandleOnInputHoldReleased(float TimeHeld);

    UFUNCTION()
    void HandleOnInputHoldLocked();
    // ~ End of Binding

private:

    UPROPERTY()
    TObjectPtr<UAbilityTask_WaitInputRelease> _HoldReleaseTask;

    UPROPERTY()
    TObjectPtr<UAbilityTask_WaitDelay> _HoldLockTask;

};

// Module Macro
#undef REN_API

