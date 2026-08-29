// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EquipmentAbility_Skill.h"

// Engine Headers
#include "AbilitySystemComponent.h"

// Project Headers
#include "Core/EquipmentSettings.h"
#include "System/EquipmentController.h"


void UEquipmentAbility_Skill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UEquipmentAbility_Skill::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
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

bool UEquipmentAbility_Skill::CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	UObject* Controller = GetSourceObject(Handle, ActorInfo);
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	const UEquipmentSettings* Settings = UEquipmentSettings::Get();

	TArray<FActiveGameplayEffectHandle> Handles = ASC->GetActiveEffectsWithAllTags(FGameplayTagContainer(Settings->EquipmentCooldownTag));
	for (const FActiveGameplayEffectHandle& Item : Handles)
	{
		const FActiveGameplayEffect* Effect = ASC->GetActiveGameplayEffect(Item);
		if (!Effect)
		{
			continue;
		}

		UObject* SourceController = Effect->Spec.GetContext().GetSourceObject();
		if (SourceController == Controller)
		{
			return false;
		}
	}

	return true;
}

void UEquipmentAbility_Skill::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	const FGameplayAbilitySpec* AbilitySpec = GetCurrentAbilitySpec();
	UEquipmentController* Controller = Cast<UEquipmentController>(GetSourceObject(Handle, ActorInfo));
	if (!AbilitySpec || !Controller)
	{
		return;
	}

	if (CooldownGameplayEffectClass && (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo)))
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, CooldownGameplayEffectClass, GetAbilityLevel());
		if (SpecHandle.IsValid())
		{
			const UEquipmentSettings* Settings = UEquipmentSettings::Get();

			SpecHandle.Data->GetContext().AddSourceObject(Controller);
			SpecHandle.Data->AppendDynamicAssetTags(FGameplayTagContainer(Settings->EquipmentCooldownTag));
			SpecHandle.Data->SetStackCount(1);

			ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
		}
	}
}

