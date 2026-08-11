// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Ability/CharacterDeadAbility.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"

// Project Headers
#include "Actor/CharacterBase.h"
#include "Core/CharacterSettings.h"


UCharacterDeadAbility::UCharacterDeadAbility() 
{
    const UCharacterSettings* Settings = UCharacterSettings::Get();
    DeadStateTag = Settings->DeadStateTag;

    if (DeadStateTag.IsValid())
    {
        FAbilityTriggerData TriggerData;
        TriggerData.TriggerTag = DeadStateTag;
        TriggerData.TriggerSource = EGameplayAbilityTriggerSource::OwnedTagAdded;

        AbilityTriggers.Add(TriggerData);
    }

    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UCharacterDeadAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
		return false;
    }
    return DeadStateTag.IsValid();
}

void UCharacterDeadAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
    }

    ACharacterBase* Character = Cast<ACharacterBase>(ActorInfo->AvatarActor.Get());
    UAbilitySystemComponent* AbilitySystem = ActorInfo->AbilitySystemComponent.Get();
    if(!IsValid(Character) || !IsValid(AbilitySystem))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

    MeshTransform = Character->GetMesh()->GetRelativeTransform();

    Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Character->GetMesh()->SetSimulatePhysics(true);
    Character->OnCharacterDied.Broadcast();

    AbilitySystem->RegisterGameplayTagEvent(DeadStateTag).AddUObject(this, &UCharacterDeadAbility::HandleOnTagChanged);
}

void UCharacterDeadAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    ACharacterBase* Character = Cast<ACharacterBase>(ActorInfo->AvatarActor.Get());
    if (IsValid(Character))
    {
        Character->GetMesh()->SetSimulatePhysics(false);
		Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		Character->GetMesh()->SetRelativeTransform(MeshTransform);
        Character->OnCharacterRevived.Broadcast();
    }

    UAbilitySystemComponent* AbilitySystem = ActorInfo->AbilitySystemComponent.Get();
    if (IsValid(AbilitySystem) && DeadStateTag.IsValid())
    {
        AbilitySystem->RegisterGameplayTagEvent(DeadStateTag).RemoveAll(this);
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCharacterDeadAbility::HandleOnTagChanged(const FGameplayTag Tag, int Count)
{
    if (Tag == DeadStateTag && Count == 0)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
	}
}

