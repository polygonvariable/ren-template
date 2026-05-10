// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/CharacterHealthComponent.h"

// Engine Headers
#include "AbilitySystemComponent.h"

// Project Headers
#include "Settings/CharacterSettings.h"


void UCharacterHealthComponent::InitializeManager()
{
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponent();
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->GenericGameplayEventCallbacks.FindOrAdd(Event_RevivedTag).AddUObject(this, &UCharacterHealthComponent::HandleOnRevived);
		AbilitySystem->GenericGameplayEventCallbacks.FindOrAdd(Event_DiedTag).AddUObject(this, &UCharacterHealthComponent::HandleOnDied);

		bool bFound = false;
		float Health = AbilitySystem->GetGameplayAttributeValue(HealthAttribute, bFound);

		if (bFound)
		{
			if (Health > 0.0f)
			{
				HandleOnRevived(nullptr);
			}
			else
			{
				HandleOnDied(nullptr);
			}
		}
	}
}

void UCharacterHealthComponent::DeinitializeManager()
{
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponent();
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->GenericGameplayEventCallbacks.FindOrAdd(Event_RevivedTag).RemoveAll(this);
		AbilitySystem->GenericGameplayEventCallbacks.FindOrAdd(Event_DiedTag).RemoveAll(this);
	}
}

void UCharacterHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeManager();
}

void UCharacterHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DeinitializeManager();
	Super::EndPlay(EndPlayReason);
}

void UCharacterHealthComponent::HandleOnRevived(const FGameplayEventData* EventData)
{
	const UCharacterSettings* Settings = UCharacterSettings::Get();
	ApplyGameplayEffect(Settings->AliveEffectClass);
}

void UCharacterHealthComponent::HandleOnDied(const FGameplayEventData* EventData)
{
	const UCharacterSettings* Settings = UCharacterSettings::Get();
	ApplyGameplayEffect(Settings->DeadEffectClass);
}

void UCharacterHealthComponent::ApplyGameplayEffect(const TSubclassOf<UGameplayEffect>& EffectClass)
{
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponent();
	if (!IsValid(EffectClass) || !IsValid(AbilitySystem))
	{
		return;
	}

	FGameplayEffectContextHandle ContextHandle = AbilitySystem->MakeEffectContext();
	FGameplayEffectSpecHandle Spec = AbilitySystem->MakeOutgoingSpec(EffectClass, 1.0f, ContextHandle);
	if (Spec.IsValid())
	{
		AbilitySystem->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
}

UAbilitySystemComponent* UCharacterHealthComponent::GetAbilitySystemComponent() const
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner))
	{
		return nullptr;
	}
	return Owner->FindComponentByClass<UAbilitySystemComponent>();
}
