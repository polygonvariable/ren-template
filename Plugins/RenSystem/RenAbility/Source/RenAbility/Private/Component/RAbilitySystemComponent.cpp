// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/RAbilitySystemComponent.h"

// Engine Headers
#include "GameFramework/Character.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"


FGameplayAbilitySpecHandle URAbilitySystemComponent::GiveAbilityWithDynamicTags(TSubclassOf<UGameplayAbility> AbilityClass, int Level, int InputID, const FGameplayTagContainer& DynamicTags)
{
	if (!IsValid(AbilityClass))
	{
		return FGameplayAbilitySpecHandle();
	}

	FGameplayAbilitySpec AbilitySpec = BuildAbilitySpecFromClass(AbilityClass, Level, InputID);
	if (!IsValid(AbilitySpec.Ability))
	{
		return FGameplayAbilitySpecHandle(); 
	}
	
	AbilitySpec.DynamicAbilityTags.AppendTags(DynamicTags);
	
	return GiveAbility(AbilitySpec);
}

void URAbilitySystemComponent::AddAggregatedActor(const FGameplayAttribute& Attribute, AActor* Actor)
{
	if (!IsValid(Actor)) return;

	TSet<TWeakObjectPtr<AActor>>& Actors = AggregatedActors.FindOrAdd(Attribute);
	Actors.Add(Actor);

	OnAggregatedRefresh.Broadcast();
	OnAggregatedActorAdded.Broadcast(Actor);
}

void URAbilitySystemComponent::RemoveAggregatedActor(const FGameplayAttribute& Attribute, AActor* Actor)
{
	if (!IsValid(Actor)) return;

	TSet<TWeakObjectPtr<AActor>>& Actors = AggregatedActors.FindOrAdd(Attribute);
	Actors.Remove(Actor);

	OnAggregatedRefresh.Broadcast();
	OnAggregatedActorRemoved.Broadcast(Actor);
}

float URAbilitySystemComponent::GetAggregatedNumericAttribute(const FGameplayAttribute& Attribute)
{
	float Value = GetNumericAttribute(Attribute);

	const TSet<TWeakObjectPtr<AActor>>& Actors = AggregatedActors.FindRef(Attribute);

	for (TWeakObjectPtr<AActor> Actor : Actors)
	{
		if (!Actor.IsValid()) continue;
		if (UAbilitySystemComponent* ASC = Actor->GetComponentByClass<UAbilitySystemComponent>())
		{
			Value += ASC->GetNumericAttribute(Attribute);
		}
	}

	return Value;
}

void URAbilitySystemComponent::BP_AddReplicatedGameplayTags(const FGameplayTagContainer& Tags)
{
	AddReplicatedLooseGameplayTags(Tags);
}

void URAbilitySystemComponent::BP_RemoveReplicatedGameplayTags(const FGameplayTagContainer& Tags)
{
	RemoveReplicatedLooseGameplayTags(Tags);
}





void URAbilitySystemComponent::BP_CancelAbilitiesByTags(const FGameplayTagContainer& WithTags)
{/*
	TArray<FGameplayAbilitySpecHandle> Abilities;
	this->GetAllAbilities(Abilities);

	for(FGameplayAbilitySpecHandle& SpecHandle : Abilities)
	{
		if (SpecHandle.IsValid())
		{
			FGameplayAbilitySpec* AbilitySpec = this->FindAbilitySpecFromHandle(SpecHandle, EConsiderPending::None);
			if (!AbilitySpec)
			{
				continue;
			}
			AbilitySpec->Level++;
			MarkAbilitySpecDirty(*AbilitySpec);
		}
	}*/

	CancelAbilities(&WithTags);
}



UAnimInstance* URAbilitySystemComponent::GetActorAnimInstance()
{
	if (!AbilityActorInfo.IsValid())
	{
		return nullptr;
	}
	return AbilityActorInfo->GetAnimInstance();
}


float URAbilitySystemComponent::AnimPlayMontage(UAnimMontage* Montage, float PlayRate, float StartTime, bool bStopAllMontages)
{
	if (UAnimInstance* AnimInstance = GetActorAnimInstance())
	{
		if (!AnimInstance->Montage_IsPlaying(Montage))
		{
			return AnimInstance->Montage_Play(Montage, PlayRate, EMontagePlayReturnType::MontageLength, StartTime, bStopAllMontages);
		}
	}
	return 0.0f;
}

void URAbilitySystemComponent::AnimStopMontage(UAnimMontage* Montage, float BlendOutTime)
{
	if (UAnimInstance* AnimInstance = GetActorAnimInstance())
	{
		AnimInstance->Montage_Stop(BlendOutTime, Montage);
	}
}

