// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Ability/EntityJumpAbility.h"

// Engine Headers
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"

// Project Headers



void UEntityJumpAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (ActorInfo || CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        AActor* AvatarActor = ActorInfo->AvatarActor.Get();
        ACharacter* Character = Cast<ACharacter>(AvatarActor);

        if (IsValid(Character))
        {
            Character->Jump();
        }
    }

    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

