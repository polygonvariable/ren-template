// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Attributes/HealthAttributeSet.h"

// Engine Headers
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "NativeGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "InstancedStruct.h"
#include "Net/UnrealNetwork.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"

#include "RCoreTag/Public/AttributeTags.h"

#include "EffectSubsystem.h"



bool UHealthAttribute::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}
	
	//if (UAbilitySystemComponent* AbilityComponent = GetOwningAbilitySystemComponent())
	//{
	//	FGameplayTagContainer Tags;
	//	Tags.AddTag(TAG_Attribute_State_Life_Immortal);
	//	return !AbilityComponent->HasAnyMatchingGameplayTags(Tags);
	//}
	//return false;

	return true;
}

void UHealthAttribute::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, FLT_MAX);
	}
}


void UHealthAttribute::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);


	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		float Magnitude = Data.EvaluatedData.Magnitude;
		bool bEffectIsDamage = Magnitude < 0.0f;

		FGameplayTagContainer AssetTags;
		Data.EffectSpec.GetAllAssetTags(AssetTags);

		AActor* Source = Data.EffectSpec.GetContext().GetOriginalInstigator();
		AActor* Target = Data.Target.AbilityActorInfo->AvatarActor.Get();

		//if (AssetTags.HasTag(TAG_Effect_Damage_Magical))
		//{
		//	PRINT_WARNING(LogTemp, 5.0f, TEXT("Magical Damage"));
		//}
		//else if(AssetTags.HasTag(TAG_Effect_Damage_Physical))
		//{
		//	PRINT_WARNING(LogTemp, 5.0f, TEXT("Physical Damage"));
		//}
		//else
		//{
		//	PRINT_WARNING(LogTemp, 5.0f, TEXT("Unknown Damage"));
		//}

		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

		if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
		{
			/*FGameplayTagContainer AliveTags;
			AliveTags.AddTag(TAG_Attribute_State_Life_Alive);
			FGameplayTagContainer DeathTags;
			DeathTags.AddTag(TAG_Attribute_State_Life_Death);*/

			
			/*if (UGameplayEventSubsystem* EffectSubsystem = UWorld::GetSubsystem<UGameplayEventSubsystem>(GetWorld()))
			{
				EffectSubsystem->SendScalarEvent(TAG_Event_Damage, Magnitude);
			}*/

			/*if (UGameplayEventHandlerSubsystem* EffectHandlerSubsystem = UWorld::GetSubsystem<UGameplayEventHandlerSubsystem>(GetWorld()))
			{
				PRINT_INFO(LogTemp, 5.0f, TEXT("Send Damage Event"));
				EffectHandlerSubsystem->SendScalarEvent(TAG_Event_Damage, Magnitude, Source, Target);
			}*/


			/*
			 * TODO:
			 * move the main logic to character as i think attribute should not call main gameplay logic,
			 * two method i thought of.
			 * 1. make the character watch for the attribute value change.
			 * 2. make a custom delegates in attribute set like (onDied, onRevived) and the character will bind to it.
			 
			if (GetHealth() <= 0.0f)
			{
				if(ASC->HasMatchingGameplayTag(TAG_Attribute_State_Life_Alive))
				{
					ASC->AddLooseGameplayTag(TAG_Attribute_State_Life_Death);
					ASC->RemoveLooseGameplayTag(TAG_Attribute_State_Life_Alive);
					PRINT_INFO(LogTemp, 1.0f, TEXT("Actor Died"));
				}
			}
			else
			{
				if (ASC->HasMatchingGameplayTag(TAG_Attribute_State_Life_Death))
				{
					ASC->AddLooseGameplayTag(TAG_Attribute_State_Life_Alive);
					ASC->RemoveLooseGameplayTag(TAG_Attribute_State_Life_Death);
					PRINT_INFO(LogTemp, 1.0f, TEXT("Actor Revived"));
				}
			}
			*/
		}
	}
}

void UHealthAttribute::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttribute, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttribute, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UHealthAttribute::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttribute, Health, OldValue);
}

void UHealthAttribute::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttribute, MaxHealth, OldValue);
}







void UStaminaAttribute::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, FLT_MAX);
	}
}

void UStaminaAttribute::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayAttribute& Attribute = Data.EvaluatedData.Attribute;

	if (Attribute == GetStaminaAttribute())
	{
		float NewValue = FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina());
		SetStamina(NewValue);
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		float NewValue = FMath::Clamp(GetMaxStamina(), 0.0f, FLT_MAX);
		SetMaxStamina(NewValue);
	}

	if (GetStamina() <= 0.f)
	{
		OnStaminaExhausted.Broadcast();
	}
	if (GetStamina() >= GetMaxStamina())
	{
		OnStaminaRefilled.Broadcast();
	}
}

void UStaminaAttribute::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UStaminaAttribute, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UStaminaAttribute, MaxStamina, COND_None, REPNOTIFY_Always);
}

void UStaminaAttribute::OnRep_Stamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UStaminaAttribute, Stamina, OldValue);
}

void UStaminaAttribute::OnRep_MaxStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UStaminaAttribute, MaxStamina, OldValue);
}











void UMovementAttribute::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetGroundSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxGroundSpeed());
	}
	else if (Attribute == GetMaxGroundSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, FLT_MAX);
	}
}

void UMovementAttribute::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayAttribute& Attribute = Data.EvaluatedData.Attribute;

	if (Attribute == GetGroundSpeedAttribute())
	{
		float NewValue = FMath::Clamp(GetGroundSpeed(), 0.0f, GetMaxGroundSpeed());
		SetGroundSpeed(NewValue);
	}
	else if (Attribute == GetMaxGroundSpeedAttribute())
	{
		float NewValue = FMath::Clamp(GetMaxGroundSpeed(), 0.0f, FLT_MAX);
		SetMaxGroundSpeed(NewValue);
	}
}

void UMovementAttribute::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UMovementAttribute, GroundSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMovementAttribute, MaxGroundSpeed, COND_None, REPNOTIFY_Always);
}

void UMovementAttribute::OnRep_GroundSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMovementAttribute, GroundSpeed, OldValue);
}

void UMovementAttribute::OnRep_MaxGroundSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMovementAttribute, MaxGroundSpeed, OldValue);
}

