// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/Attribute/HealthComponent.h"

// Engine Headers
#include "AbilitySystemComponent.h"

// Project Headers
#include "Attributes/HealthSet.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Settings/AbilitySettings.h"


void UHealthComponent::InitializeManager()
{
	AActor* Owner = GetOwner();
	if(!IsValid(Owner))
	{
		LOG_ERROR(LogTemp, TEXT("Owner is invalid"));
		return;
	}

	AbilitySystem = Owner->FindComponentByClass<UAbilitySystemComponent>();
	if (!IsValid(AbilitySystem))
	{
		LOG_ERROR(LogTemp, TEXT("AbilitySystemComponent is invalid"));
		return;
	}

	HealthSet = AbilitySystem->GetSet<UHealthSet>();
	if (!IsValid(HealthSet))
	{
		LOG_ERROR(LogTemp, TEXT("HealthSet is invalid"));
		return;
	}

	HealthSet->OnDied.AddUObject(this, &UHealthComponent::HandleOnDied);
	HealthSet->OnRevived.AddUObject(this, &UHealthComponent::HandleOnRevived);

	float Health = HealthSet->GetHealth();
	if (Health > 0.0f)
	{
		HandleOnRevived();
	}
	else
	{
		HandleOnDied();
	}
}

void UHealthComponent::DeinitializeManager()
{
	if (IsValid(HealthSet))
	{
		HealthSet->OnDied.RemoveAll(this);
		HealthSet->OnRevived.RemoveAll(this);
	}
	HealthSet = nullptr;
	AbilitySystem = nullptr;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeManager();
}

void UHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DeinitializeManager();
	Super::EndPlay(EndPlayReason);
}

void UHealthComponent::HandleOnRevived()
{
	if (!DeadHandle.IsValid() || !IsValid(AbilitySystem))
	{
		return;
	}
	AbilitySystem->RemoveActiveGameplayEffect(DeadHandle);
}

void UHealthComponent::HandleOnDied()
{
	const UAbilitySettings* Settings = UAbilitySettings::Get();
	if (!IsValid(Settings->DeadEffect) || !IsValid(AbilitySystem))
	{
		LOG_ERROR(LogTemp, TEXT("DeadEffect is invalid"));
		return;
	}

	FGameplayEffectContextHandle ContextHandle = AbilitySystem->MakeEffectContext();
	FGameplayEffectSpecHandle Spec = AbilitySystem->MakeOutgoingSpec(Settings->DeadEffect, 1.0f, ContextHandle);
	if (Spec.IsValid())
	{
		DeadHandle = AbilitySystem->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
}

