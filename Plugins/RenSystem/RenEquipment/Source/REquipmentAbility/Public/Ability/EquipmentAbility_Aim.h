// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Abilities/GameplayAbility.h"

// Generated Headers
#include "EquipmentAbility_Aim.generated.h"

// Forward Declaration
class UCameraComponent;
class USpringArmComponent;
class ACharacter;
class UGameplayEffect;

/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UEquipmentAbility_Aim : public UGameplayAbility
{

	GENERATED_BODY()

public:

    UEquipmentAbility_Aim();

    // ~ UGameplayAbility
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
    // ~ End of UGameplayAbility

protected:

    UPROPERTY(EditAnywhere)
    TSubclassOf<UGameplayEffect> AimEffectClass;

    UPROPERTY(EditAnywhere)
    float SocketLength = 64.0f;

    UPROPERTY(EditAnywhere)
    FVector SocketOffset = FVector(0, 45, 0);

    UPROPERTY(EditAnywhere)
    float FieldOfView = 55.0f;

    FActiveGameplayEffectHandle AimHandle;


    UFUNCTION(BlueprintNativeEvent)
    USpringArmComponent* GetSprintArmComponent(ACharacter* Character) const;
    virtual USpringArmComponent* GetSprintArmComponent_Implementation(ACharacter* Character) const;

    UFUNCTION(BlueprintNativeEvent)
    UCameraComponent* GetCameraComponent(ACharacter* Character) const;
    virtual UCameraComponent* GetCameraComponent_Implementation(ACharacter* Character) const;

    // ~ Binding
    UFUNCTION()
    void HandleOnInputReleased(float TimeHeld);
    // ~ End of Binding

private:

    float _PreviousSocketLength;
    FVector _PreviousSocketOffset;
    float _PreviousFieldOfView;

};

