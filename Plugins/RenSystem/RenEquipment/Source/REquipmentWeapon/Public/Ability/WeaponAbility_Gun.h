// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Ability/EquipmentAbility_Input.h"

// Generated Headers
#include "WeaponAbility_Gun.generated.h"

// Forward Declaration
class UAnimMontage;
class UAbilityTask_WaitDelay;
class UAbilityTask_WaitInputRelease;


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UWeaponAbility_Gun : public UEquipmentAbility_Input
{

	GENERATED_BODY()

public:

    UWeaponAbility_Gun();

#if WITH_EDITOR
    // ~ UGameplayAbility
    virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
    // ~ End of UGameplayAbility
#endif

protected:

    UPROPERTY(EditAnywhere)
    TObjectPtr<UAnimMontage> SingleFireMontage;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UAnimMontage> HoldFireMontage;

    UPROPERTY(EditAnywhere)
    FName ProjectileSocket = TEXT_EMPTY;

    UPROPERTY(EditAnywhere)
    FGameplayTag AimTag;

    UPROPERTY(EditAnywhere)
    FVector2D AimOffset;

    UPROPERTY(EditAnywhere)
    float TraceDistance = 25000.0f;

    UPROPERTY(EditAnywhere)
    float HoldFireInterval = 0.5f;

    UPROPERTY()
    TObjectPtr<UAbilityTask_WaitDelay> LockTask;


    // ~ Binding
    UFUNCTION()
    void HandleOnLockFinished();
    // ~ End of Binding

    // ~ UEquipmentAbility_Input
    virtual void OnInputSingle() override;
    virtual void OnInputHold() override;
    // ~ End of UEquipmentAbility_Input

    void TraceProjectile();

};

