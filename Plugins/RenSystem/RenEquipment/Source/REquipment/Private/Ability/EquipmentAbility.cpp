// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Ability/EquipmentAbility.h"

// Engine Headers
#include "AbilitySystemComponent.h"

// Project Headers
#include "Settings/EquipmentSettings.h"
#include "Controller/EquipmentController.h"
#include "Actor/EquipmentActor.h"



bool UEquipmentStateAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	FGameplayAbilitySpec* AbilitySpec = GetCurrentAbilitySpec();
	if (!AbilitySpec || !AbilitySpec->SourceObject.IsValid())
	{
		return false;
	}

	FGameplayTag EquipmentTag = AbilitySpec->DynamicAbilityTags.First();

	const UEquipmentSettings* Settings = UEquipmentSettings::Get();
	const FEquipmentTagData* TagData = Settings->GetTagData(EquipmentTag);
	if (!TagData)
	{
		return false;
	}

	UAbilitySystemComponent* AbilitySystem = ActorInfo->AbilitySystemComponent.Get();
	if (!AbilitySystem)
	{
		return false;
	}

	// Only activate ability if the equipment state tag is not present
	return !AbilitySystem->HasMatchingGameplayTag(TagData->StateTag);
}

#if WITH_EDITOR
EDataValidationResult UEquipmentStateAbility::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);
	if (Result == EDataValidationResult::Valid)
	{
		if (InstancingPolicy != EGameplayAbilityInstancingPolicy::InstancedPerActor)
		{
			Result = EDataValidationResult::Invalid;
			Context.AddError(FText::FromString("EquipmentStateAbility must be instanced per actor"));
		}
	}
	return Result;
}
#endif

void UEquipmentStateAbility::EnableEquipment()
{
	UEquipmentController* Controller = GetController();
	if (IsValid(Controller))
	{
		Controller->ActivateEquipment();
	}
}

void UEquipmentStateAbility::DisableEquipment()
{
	UEquipmentController* Controller = GetController();
	if (IsValid(Controller))
	{
		Controller->DeactivateEquipment();
	}
}

UEquipmentController* UEquipmentStateAbility::GetController() const
{
	FGameplayAbilitySpec* AbilitySpec = GetCurrentAbilitySpec();
	if (!AbilitySpec)
	{
		return nullptr;
	}
	return Cast<UEquipmentController>(AbilitySpec->SourceObject.Get());
}




bool UEquipmentWeaponAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	FGameplayAbilitySpec* AbilitySpec = GetCurrentAbilitySpec();
	if (!AbilitySpec || !AbilitySpec->SourceObject.IsValid())
	{
		return false;
	}

	FGameplayTag EquipmentTag = AbilitySpec->DynamicAbilityTags.First();

	const UEquipmentSettings* Settings = UEquipmentSettings::Get();
	const FEquipmentTagData* TagData = Settings->GetTagData(EquipmentTag);
	if (!TagData)
	{
		return false;
	}

	UAbilitySystemComponent* AbilitySystem = ActorInfo->AbilitySystemComponent.Get();
	if (!AbilitySystem)
	{
		return false;
	}

	// Only activate ability if the equipment state tag is present
	return AbilitySystem->HasMatchingGameplayTag(TagData->StateTag);
}

#if WITH_EDITOR
EDataValidationResult UEquipmentWeaponAbility::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);
	if (Result == EDataValidationResult::Valid)
	{
		if (InstancingPolicy != EGameplayAbilityInstancingPolicy::InstancedPerActor)
		{
			Result = EDataValidationResult::Invalid;
			Context.AddError(FText::FromString("EquipmentWeaponAbility must be instanced per actor"));
		}
	}
	return Result;
}
#endif


UEquipmentController* UEquipmentWeaponAbility::GetEquipmentController() const
{
	FGameplayAbilitySpec* AbilitySpec = GetCurrentAbilitySpec();
	if (!AbilitySpec)
	{
		return nullptr;
	}
	return Cast<UEquipmentController>(AbilitySpec->SourceObject.Get());
}

AEquipmentActor* UEquipmentWeaponAbility::GetEquipmentActor() const
{
	UEquipmentController* Controller = GetEquipmentController();
	if (!IsValid(Controller))
	{
		return nullptr;
	}
	return Cast<AEquipmentActor>(Controller->GetEquipmentActor());
}







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






