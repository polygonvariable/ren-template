// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Attributes/HealthSet.h"

// Engine Headers
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "Net/UnrealNetwork.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"
#include "RenAbility/Public/EffectSubsystem.h"

// Declare Gameplay Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Health_Damage, "Event.Attribute.Health.Damage");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Health_Heal, "Event.Attribute.Health.Heal");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Health_Immunity, "Event.Attribute.Health.Immunity");



bool UHealthSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	return true;
}

void UHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMinHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}

void UHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	const FGameplayAttribute& Attribute = Data.EvaluatedData.Attribute;

	if (Attribute == GetDamageAttribute())
	{
		float CachedValue = GetDamage();
		SetDamage(0.0f);

		UpdateHealth(CachedValue, -1.0f);

		UGameplayNotifySubsystem* GameplayNotify = GetGameplayNotifySubsystem();
		if (IsValid(GameplayNotify))
		{
			FGameplayNotifyPacket Packet = FGameplayNotifyPacket(CachedValue);
			GameplayNotify->SendNotify(TAG_Attribute_Health_Damage, Packet, false);
		}

		UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
		if (IsValid(ASC))
		{
			FGameplayEventData EventData = FGameplayEventData();
			EventData.EventMagnitude = CachedValue;
			EventData.ContextHandle = Data.EffectSpec.GetContext();

			ASC->HandleGameplayEvent(TAG_Attribute_Health_Damage, &EventData);
		}
	}
	else if (Attribute == GetHealAttribute())
	{
		float CachedValue = GetHeal();
		SetHeal(0.0f);
		
		UpdateHealth(CachedValue, 1.0f);
	}
	else if (Attribute == GetMinHealthAttribute())
	{
		float NewMin = FMath::Min(GetMinHealth(), GetMaxHealth());
		SetMinHealth(NewMin);

		float NewHealth = FMath::Max(GetHealth(), NewMin);
		SetHealth(NewHealth);
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		float NewMax = FMath::Max(GetMaxHealth(), GetMinHealth());
		SetMaxHealth(NewMax);

		float NewHealth = FMath::Min(GetHealth(), NewMax);
		SetHealth(NewHealth);
	}
}

void UHealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UHealthSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthSet, MinHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UHealthSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthSet, Health, OldValue);
}

void UHealthSet::OnRep_MinHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthSet, MinHealth, OldValue);
}

void UHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthSet, MaxHealth, OldValue);
}

void UHealthSet::UpdateHealth(float Value, float Multiplier)
{
	if (Value > 0)
	{
		float NewValue = GetHealth() + (Value * Multiplier);
		float NewHealth = FMath::Clamp(NewValue, GetMinHealth(), GetMaxHealth());
		SetHealth(NewHealth);
	}
}

bool UHealthSet::IsDead() const
{
	return bIsDead;
}

void UHealthSet::UpdateDeathState()
{
	const bool bShouldBeDead = GetHealth() <= GetMinHealth() && GetMinHealth() <= 0.0f;

	if (bIsDead == bShouldBeDead)
	{
		return;
	}

	bIsDead = bShouldBeDead;
}

UGameplayNotifySubsystem* UHealthSet::GetGameplayNotifySubsystem()
{
	UGameplayNotifySubsystem* GameplayNotify = GameplayNotifySubsystem.Get();

	if (!IsValid(GameplayNotify))
	{
		UWorld* World = GetWorld();
		if (IsValid(World))
		{
			GameplayNotify = World->GetSubsystem<UGameplayNotifySubsystem>();
			GameplayNotifySubsystem = GameplayNotify;
		}
	}

	return GameplayNotify;
}

