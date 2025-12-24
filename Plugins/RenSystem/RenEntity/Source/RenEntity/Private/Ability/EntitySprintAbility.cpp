// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Ability/EntitySprintAbility.h"

// Engine Headers
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"

// Project Headers



UCharacterMovementComponent* UEntitySprintAbility::GetCharacterMovement() const
{
    AActor* AvatarActor = CurrentActorInfo->AvatarActor.Get();
    ACharacter* Character = Cast<ACharacter>(AvatarActor);
    if (!IsValid(Character))
    {
        return nullptr;
    }

    return Character->GetCharacterMovement();
}

void UEntitySprintAbility::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
    float NewValue = Data.NewValue;
    if (NewValue <= 0.0f)
	{
        CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false);
	}
}


bool UEntitySprintAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

    if (!StaminaAttribute.IsValid())
    {
		return false;
    }

    // Avatar actor is already validated in Super::CanActivateAbility
    AActor* Avatar = ActorInfo->AvatarActor.Get();

    FVector Velocity = Avatar->GetVelocity();
    float Speed = Velocity.Size2D();
    
    return Speed > SprintMinSpeed;
}

void UEntitySprintAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (!ActorInfo || !CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        return;
    }

    UCharacterMovementComponent* CharacterMovement = GetCharacterMovement();
    UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
    if (!IsValid(CharacterMovement) || !IsValid(ASC))
    {
        return;
    }

    ASC->AddReplicatedLooseGameplayTag(SprintTag);

    PreviousSpeed = CharacterMovement->MaxWalkSpeed;

    CharacterMovement->MaxWalkSpeed = SprintSpeed;
    
    FOnGameplayAttributeValueChange& StaminaDelegate = ASC->GetGameplayAttributeValueChangeDelegate(StaminaAttribute);
    StaminaDelegate.RemoveAll(this);
    StaminaDelegate.AddUObject(this, &UEntitySprintAbility::OnStaminaChanged);

    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UEntitySprintAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    if (ActorInfo)
    {
        UCharacterMovementComponent* CharacterMovement = GetCharacterMovement();
        UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();

        if (IsValid(CharacterMovement) && IsValid(ASC))
        {
            ASC->RemoveReplicatedLooseGameplayTag(SprintTag);

            FOnGameplayAttributeValueChange& StaminaDelegate = ASC->GetGameplayAttributeValueChangeDelegate(StaminaAttribute);
            StaminaDelegate.RemoveAll(this);

            CharacterMovement->MaxWalkSpeed = PreviousSpeed;
        }
    }

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

