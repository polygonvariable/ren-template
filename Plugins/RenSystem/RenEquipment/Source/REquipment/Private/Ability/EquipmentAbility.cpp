// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Ability/EquipmentAbility.h"

// Engine Headers
#include "AbilitySystemComponent.h"

// Project Headers
#include "Settings/EquipmentSettings.h"


void UEquipmentSkillAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	const FGameplayAbilitySpec* AbilitySpec = GetCurrentAbilitySpec();
	if (AbilitySpec && !EquipmentCooldownTags.IsValid())
	{
		const FGameplayTag& EquipmentTag = AbilitySpec->DynamicAbilityTags.First();
		const FEquipmentTagData* TagData = UEquipmentSettings::GetTagDataByAbility(EquipmentTag);
		if (TagData)
		{
			EquipmentCooldownTags = FGameplayTagContainer(TagData->CooldownTag);
		}
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

const FGameplayTagContainer* UEquipmentSkillAbility::GetCooldownTags() const
{
	return &EquipmentCooldownTags;
}

void UEquipmentSkillAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	if (CostGameplayEffectClass)
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, CostGameplayEffectClass, GetAbilityLevel());
		if (SpecHandle.IsValid())
		{
			SpecHandle.Data->SetStackCount(1);
			ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
		}
	}
}

void UEquipmentSkillAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	const FGameplayAbilitySpec* AbilitySpec = GetCurrentAbilitySpec();
	if (!AbilitySpec)
	{
		return;
	}

	if (CooldownGameplayEffectClass && EquipmentCooldownTags.IsValid() && (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo)))
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, CooldownGameplayEffectClass, GetAbilityLevel());
		if (SpecHandle.IsValid())
		{
			SpecHandle.Data->DynamicGrantedTags.AppendTags(EquipmentCooldownTags);
			SpecHandle.Data->SetStackCount(1);
			
			ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
		}
	}
}
