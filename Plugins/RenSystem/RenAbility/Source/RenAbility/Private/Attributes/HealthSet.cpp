// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Attributes/HealthSet.h"

// Engine Headers
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "Net/UnrealNetwork.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"
#include "RenAbility/Public/Notify/GameplayNotifyPacket.h"
#include "RenAbility/Public/Notify/GameplayNotifySubsystem.h"

// Declare Gameplay Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Health_Damaged, "Attribute.Health.Damaged");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Health_Healed, "Attribute.Health.Healed");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Health_Immunity, "Attribute.Health.Immunity");

UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Health_Fallen, "Attribute.Health.Fallen");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Health_Restored, "Attribute.Health.Restored");



bool UHealthSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	return true;
}

void UHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	const FGameplayAttribute& Attribute = Data.EvaluatedData.Attribute;

	if (Attribute == GetDamageAttribute())
	{
		HandleDamage(Data);
	}
	else if (Attribute == GetHealAttribute())
	{
		HandleHeal(Data);
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

	UpdateHealthState();
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




void UHealthSet::UpdateHealthState()
{
	const bool bShouldBeDead = GetHealth() <= GetMinHealth() && GetMinHealth() <= 0.0f;

	if (bIsDead == bShouldBeDead)
	{
		return;
	}

	bIsDead = bShouldBeDead;

	BroadcastHealthState();
}

void UHealthSet::BroadcastHealthState()
{
	FGameplayEventData EventData;
	EventData.Target = GetOwningActor();

	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();

	if (IsValid(ASC))
	{
		if (bIsDead)
		{
			ASC->HandleGameplayEvent(TAG_Attribute_Health_Fallen, &EventData);
		}
		else
		{
			ASC->HandleGameplayEvent(TAG_Attribute_Health_Restored, &EventData);
		}
	}
}



void UHealthSet::HandleDamage(const FGameplayEffectModCallbackData& Data)
{
	float PreviousHealth = GetHealth();
	float CachedDamage = GetDamage();
	SetDamage(0.0f);

	UpdateHealth(CachedDamage, -1.0f);

	float ChangedHealth = PreviousHealth - GetHealth();
	if (FMath::IsNearlyEqual(ChangedHealth, 0.0f))
	{
		return;
	}

	BroadcastHealthChanged(Data, CachedDamage, TAG_Attribute_Health_Damaged);
}

void UHealthSet::HandleHeal(const FGameplayEffectModCallbackData& Data)
{
	float PreviousHealth = GetHealth();
	float CachedHeal = GetHeal();
	SetHeal(0.0f);

	UpdateHealth(CachedHeal, 1.0f);

	float ChangedHealth = PreviousHealth - GetHealth();
	if (FMath::IsNearlyEqual(ChangedHealth, 0.0f))
	{
		return;
	}

	BroadcastHealthChanged(Data, CachedHeal, TAG_Attribute_Health_Healed);
}

void UHealthSet::BroadcastHealthChanged(const FGameplayEffectModCallbackData& Data, float Magnitude, const FGameplayTag& EventTag)
{
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
	if (IsValid(ASC))
	{
		FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();

		FGameplayEventData EventData;
		EventData.EventMagnitude = Magnitude;
		EventData.ContextHandle = Context;

		ASC->HandleGameplayEvent(EventTag, &EventData);
	}
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

/*UGameplayNotifySubsystem* UHealthSet::GetGameplayNotifySubsystem()
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
}*/

