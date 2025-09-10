// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Attributes/HealthAttributeSet.h"

// Engine Headers
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "NativeGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "InstancedStruct.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"

#include "RCoreTag/Public/AttributeTags.h"

#include "EffectSubsystem.h"



bool UHealthAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
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

void UHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetHealthMax());
	}
	else if (Attribute == GetHealthMaxAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, FLT_MAX);
	}
}


void UHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
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

		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetHealthMax()));

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

		}
	}
}

