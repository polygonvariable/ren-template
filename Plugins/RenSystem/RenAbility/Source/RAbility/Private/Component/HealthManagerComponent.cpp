// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/HealthManagerComponent.h"

// Engine Headers
#include "AbilitySystemComponent.h"

// Project Headers
#include "Ability/Attribute/HealthSet.h"
#include "Core/AbilitySettings.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


UHealthManagerComponent::UHealthManagerComponent()
{
	bAutoActivate = false;
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UHealthManagerComponent::Activate(bool bReset)
{
	Super::Activate(bReset);

	if (HasBegunPlay() && IsActive())
	{
		const float Health = HealthSet->GetHealth();
		if (Health > 0.0f)
		{
			HandleOnRevived();
		}
		else
		{
			HandleOnDied();
		}
	}
}

void UHealthManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (!IsValid(Owner))
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

	HealthSet->OnDied.AddUObject(this, &UHealthManagerComponent::HandleOnDied);
	HealthSet->OnRevived.AddUObject(this, &UHealthManagerComponent::HandleOnRevived);
}

void UHealthManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(HealthSet))
	{
		HealthSet->OnDied.RemoveAll(this);
		HealthSet->OnRevived.RemoveAll(this);
	}
	HealthSet = nullptr;
	AbilitySystem = nullptr;

	Super::EndPlay(EndPlayReason);
}


void UHealthManagerComponent::HandleOnRevived()
{
	if (!DeadHandle.IsValid() || !IsValid(AbilitySystem))
	{
		return;
	}
	AbilitySystem->RemoveActiveGameplayEffect(DeadHandle);
}

void UHealthManagerComponent::HandleOnDied()
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

