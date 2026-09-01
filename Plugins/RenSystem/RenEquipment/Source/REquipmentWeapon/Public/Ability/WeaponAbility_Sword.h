// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Ability/EquipmentAbility_Input.h"

// Generated Headers
#include "WeaponAbility_Sword.generated.h"

// Forward Declaration
class UAnimMontage;
class UAbilityTask_WaitDelay;
class UAbilityTask_WaitInputRelease;
class UAbilityTask_WaitGameplayEvent;


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UWeaponAbility_Sword : public UEquipmentAbility_Input
{

	GENERATED_BODY()

public:

    UWeaponAbility_Sword();

    // ~ UGameplayAbility
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
    // ~ End of UGameplayAbility

#if WITH_EDITOR
    // ~ UGameplayAbility
    virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
    // ~ End of UGameplayAbility
#endif

protected:

    UPROPERTY(EditAnywhere)
    TArray<TObjectPtr<UAnimMontage>> RapidAttacks;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UAnimMontage> ChargedAttack;

    UPROPERTY(EditAnywhere)
    float TraceDistance = 300.0f;

    UPROPERTY(EditAnywhere)
    FGameplayTag SwapEventTag;

    UPROPERTY(EditAnywhere)
    FGameplayTag AttackEventTag;

    UPROPERTY(EditAnywhere)
    TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;

    UPROPERTY()
    TObjectPtr<UAbilityTask_WaitGameplayEvent> SwapTask;

    UPROPERTY()
    TObjectPtr<UAbilityTask_WaitGameplayEvent> AttackTask;

    int MontageIndex = 0;


    // ~ Binding
    UFUNCTION()
    void HandleOnSwapEventReceived(FGameplayEventData Payload);

    UFUNCTION()
    void HandleOnAttackEventReceived(FGameplayEventData Payload);

    UFUNCTION()
    void HandleOnMontageEvent();
    // ~ End of Binding

    // ~ EquipmentAbility_Input
    virtual void OnInputSingle() override;
    virtual void OnInputHold() override;
    // ~ End of EquipmentAbility_Input

    void PlayAndRegisterMontage(UAnimMontage* Montage);

};

