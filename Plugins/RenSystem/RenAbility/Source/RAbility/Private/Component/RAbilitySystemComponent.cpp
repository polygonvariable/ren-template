// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/RAbilitySystemComponent.h"

// Engine Headers
#include "GameFramework/Character.h"

// Project Headers
#include "Log/LogMacro.h"



FGameplayAbilitySpecHandle URAbilitySystemComponent::BP_TryGiveAbilityWithSource(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level, int32 InputID, const FGameplayTagContainer& Tags, UObject* SourceObject)
{
	if (!IsValid(AbilityClass)|| !IsValid(SourceObject))
	{
		return FGameplayAbilitySpecHandle();
	}

	FGameplayAbilitySpec Spec(AbilityClass, Level, InputID, SourceObject);
	Spec.DynamicAbilityTags.AppendTags(Tags);

	return GiveAbility(Spec);
}

void URAbilitySystemComponent::BP_CancelAbilitiesWithTags(const FGameplayTagContainer& Tags)
{
	CancelAbilities(&Tags);
}

void URAbilitySystemComponent::BP_CancelAbilitiesWithoutTags(const FGameplayTagContainer& Tags)
{
	CancelAbilities(nullptr, &Tags);
}

void URAbilitySystemComponent::BP_CancelAbilitiesWithDynamicTags(const FGameplayTagContainer& Tags)
{
	const TArray<FGameplayAbilitySpec>& AbilitiesSpecs = ActivatableAbilities.Items;

	TArray<FGameplayAbilitySpecHandle> ToRemove;

	for (const FGameplayAbilitySpec& Spec : AbilitiesSpecs)
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

	this->GiveAbility(FGameplayAbilitySpec());
}

void URAbilitySystemComponent::BP_CancelAbilityWithHandle(const FGameplayAbilitySpecHandle& Handle)
{
	CancelAbilityHandle(Handle);
}


bool URAbilitySystemComponent::BP_IsGameplayEffectActive(UPARAM(ref) FActiveGameplayEffectHandle& Handle) const
{
	const FActiveGameplayEffect* Effect = GetActiveGameplayEffect(Handle);
	return (Effect != nullptr);
}


bool URAbilitySystemComponent::BP_IsGameplayEffectInhibited(UPARAM(ref) FActiveGameplayEffectHandle& Handle) const
{
	const FActiveGameplayEffect* Effect = GetActiveGameplayEffect(Handle);
	if (!Effect)
	{
		return false;
	}
	return Effect->bIsInhibited;
}

void URAbilitySystemComponent::BP_InhibitGameplayEffect(UPARAM(ref) FActiveGameplayEffectHandle& InHandle, bool bInInhibit, bool bInInvokeCue, FActiveGameplayEffectHandle& OutHandle)
{
	OutHandle = SetActiveGameplayEffectInhibit(MoveTemp(InHandle), bInInhibit, bInInvokeCue);
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
