// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Abilities/EquipmentAbility.h"

// Engine Headers
#include "Kismet/GameplayStatics.h"

// Project Headers
#include "Component/RAbilitySystemComponent.h"
#include "RCoreLibrary/Public/LogMacro.h"



void UEquipmentAbility::OnTagChanged(const FGameplayTag Tag, int NewCount)
{
	if (RASC.IsValid())
	{
		if (NewCount > 0)
		{
			RASC->AnimStopMontage(UnequipMontage, BlendOut);
			RASC->AnimPlayMontage(EquipMontage, PlayRate, StartTime, bStopAllMontages);
		}
		else
		{
			RASC->AnimStopMontage(EquipMontage, BlendOut);
			RASC->AnimPlayMontage(UnequipMontage, PlayRate, StartTime, bStopAllMontages);
		}
	}
}

bool UEquipmentAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (const FGameplayAbilitySpec* Spec = GetCurrentAbilitySpec())
	{
		return Spec->DynamicAbilityTags.First().IsValid() && IsValid(EquipMontage);
	}
	return false;
}

void UEquipmentAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		const FGameplayAbilitySpec* Spec = GetCurrentAbilitySpec();
		if (!Spec)
		{
			PRINT_ERROR(LogTemp, 5.0f, TEXT("Spec is not valid"));
			return;
		}

		SlotTag = Spec->DynamicAbilityTags.First();
		if (!SlotTag.IsValid())
		{
			PRINT_ERROR(LogTemp, 5.0f, TEXT("SlotTag is not valid"));
			return;
		}

		RASC = Cast<URAbilitySystemComponent>(ASC);
		if (!RASC.IsValid())
		{
			PRINT_ERROR(LogTemp, 5.0f, TEXT("AbilitySystemComponent is not valid"));
			return;
		}

		RASC->RegisterGameplayTagEvent(SlotTag).AddUObject(this, &UEquipmentAbility::OnTagChanged);
	}
}

void UEquipmentAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (RASC.IsValid())
	{
		RASC->AnimStopMontage(EquipMontage, BlendOut);
		RASC->AnimStopMontage(UnequipMontage, BlendOut);
		RASC->RegisterGameplayTagEvent(SlotTag).RemoveAll(this);
	}
	RASC.Reset();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

