// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/RAbilitySystemComponent.h"

// Engine Headers
#include "GameFramework/Character.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"



void URAbilitySystemComponent::BP_CancelAbilitiesByTags(const FGameplayTagContainer& Tags)
{
	CancelAbilities(&Tags);
}

void URAbilitySystemComponent::BP_CancelAbilitiesWithoutTags(const FGameplayTagContainer& Tags)
{
	CancelAbilities(nullptr, &Tags);
}

void URAbilitySystemComponent::BP_CancelAbilitiesWithDynamicTags(const FGameplayTagContainer& Tags)
{
	const TArray<FGameplayAbilitySpec>& AbilitieSpecs = ActivatableAbilities.Items;

	TArray<FGameplayAbilitySpecHandle> ToRemove;

	for (const FGameplayAbilitySpec& Spec : AbilitieSpecs)
	{
		if (Spec.DynamicAbilityTags.HasAnyExact(Tags))
		{
			ToRemove.Add(Spec.Handle);
		}
	}

	for (const FGameplayAbilitySpecHandle& Handle : ToRemove)
	{
		CancelAbilityHandle(Handle);
	}
}

void URAbilitySystemComponent::BP_CancelAbilityWithHandle(const FGameplayAbilitySpecHandle& Handle)
{
	CancelAbilityHandle(Handle);
}


int32 URAbilitySystemComponent::HandleGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	int Result = Super::HandleGameplayEvent(EventTag, Payload);

	TriggerGameplayCue(EventTag, Payload);

	return Result;
}

void URAbilitySystemComponent::TriggerGameplayCue(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	if (bUseCueBinding && IsValid(CueBindingAsset))
	{
		const FGameplayCueTag* CueTag = CueBindingAsset->GetGameplayCueTag(EventTag);
		if (CueTag && CueTag->IsValid())
		{
			FGameplayCueParameters CueParameters;
			if (Payload)
			{
				CueParameters.EffectContext = Payload->ContextHandle;
			}
			
			ExecuteGameplayCue(CueTag->GameplayCueTag, CueParameters);
		}
	}
}



const FGameplayCueTag* UGameplayCueBinding::GetGameplayCueTag(FGameplayTag Tag) const
{
	return TagRedirects.Find(Tag);
}
