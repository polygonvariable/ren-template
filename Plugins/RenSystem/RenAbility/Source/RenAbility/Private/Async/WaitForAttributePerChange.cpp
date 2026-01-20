// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Async/WaitForAttributePerChange.h"

// Engine Headers
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Misc/DataValidation.h"
// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"



UWaitForAttributePerChange* UWaitForAttributePerChange::WaitForAttributePerChange(AActor* InActor, FGameplayAttribute InAttribute, EChangeType InChangeType, bool bInNegate)
{
	UWaitForAttributePerChange* NewTask = NewObject<UWaitForAttributePerChange>();
	NewTask->Actor = InActor;
	NewTask->Attribute = InAttribute;
	NewTask->ChangeType = InChangeType;
	NewTask->bNegate = bInNegate;
	return NewTask;
}

void UWaitForAttributePerChange::EndTask()
{
	UAbilitySystemComponent* TargetASC = ASC.Get();
	if (IsValid(TargetASC))
	{
		FOnGameplayAttributeValueChange& Delegate = TargetASC->GetGameplayAttributeValueChangeDelegate(Attribute);
		Delegate.RemoveAll(this);
	}

	ASC.Reset();
	Actor.Reset();

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitForAttributePerChange::HandleValueChanged(const FOnAttributeChangeData& Data)
{
	if (CheckCondition(Data.NewValue))
	{
		OldValue = Data.NewValue;
		OnChanged.Broadcast(Data.NewValue, Data.OldValue);
	}
}

bool UWaitForAttributePerChange::CheckCondition(float NewValue)
{
	bool bResult = false;
	if (ChangeType == EChangeType::Equal)
	{
		bResult = FMath::IsNearlyEqual(NewValue, OldValue);
	}
	if (ChangeType == EChangeType::NotEqual)
	{
		bResult = !FMath::IsNearlyEqual(NewValue, OldValue);
	}
	else if (ChangeType == EChangeType::GreaterThan)
	{
		bResult = NewValue > OldValue;
	}
	else if (ChangeType == EChangeType::LessThan)
	{
		bResult = NewValue < OldValue;
	}
	return bNegate ? !bResult : bResult;
}

void UWaitForAttributePerChange::Activate()
{
	AActor* TargetActor = Actor.Get();
	if (!IsValid(TargetActor))
	{
		return;
	}

	UAbilitySystemComponent* TargetASC = TargetActor->FindComponentByClass<UAbilitySystemComponent>();
	if (!IsValid(TargetASC))
	{
		return;
	}

	FOnGameplayAttributeValueChange& Delegate = TargetASC->GetGameplayAttributeValueChangeDelegate(Attribute);
	Delegate.AddUObject(this, &UWaitForAttributePerChange::HandleValueChanged);

	Actor = TargetActor;
	ASC = TargetASC;
}

void USendEventGameplayEffectComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	if (ActiveGEContainer.IsNetAuthority())
	{
		UAbilitySystemComponent* ASC = ActiveGEContainer.Owner;
		if (IsValid(ASC) && EventTag.IsValid())
		{
			ASC->HandleGameplayEvent(EventTag, &Payload);
		}
	}
}

EDataValidationResult USendEventGameplayEffectComponent::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (!EventTag.IsValid())
	{
		Context.AddError(FText::FromString("EventTag is not valid"));
		Result = EDataValidationResult::Invalid;
	}

	return Result;
}
