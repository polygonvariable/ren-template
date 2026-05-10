// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AttributeClampedUI.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Log/LogMacro.h"



void UAttributeClampedUI::RegisterAbilitySystem(UAbilitySystemComponent* AbilitySystem)
{
	UnregisterAbilitySystem();

	if (!IsValid(AbilitySystem) || !BaseAttribute.IsValid() || !MaxAttribute.IsValid())
	{
		LOG_ERROR(LogTemp, TEXT("AbilitySystemComponent, BaseAttribute or MaxAttribute is not valid"));
		return;
	}

	int CurrentValue = AbilitySystem->GetNumericAttribute(BaseAttribute);
	int MaxValue = AbilitySystem->GetNumericAttribute(MaxAttribute);
	UpdateValue(CurrentValue, MaxValue);

	AbilitySystem->GetGameplayAttributeValueChangeDelegate(BaseAttribute).AddUObject(this, &UAttributeClampedUI::HandleValueChanged);
	AbilitySystem->GetGameplayAttributeValueChangeDelegate(MaxAttribute).AddUObject(this, &UAttributeClampedUI::HandleValueChanged);

	ASC = TWeakObjectPtr<UAbilitySystemComponent>(AbilitySystem);
}

void UAttributeClampedUI::HandleValueChanged(const FOnAttributeChangeData& Data)
{
	UAbilitySystemComponent* AbilitySystem = ASC.Get();
	if (!IsValid(AbilitySystem))
	{
		LOG_ERROR(LogTemp, TEXT("AbilitySystemComponent is not valid"));
		return;
	}

	int CurrentValue = AbilitySystem->GetNumericAttribute(BaseAttribute);
	int MaxValue = AbilitySystem->GetNumericAttribute(MaxAttribute);

	UpdateValue(CurrentValue, MaxValue);
}

void UAttributeClampedUI::UpdateValue(int Current, int Max)
{
	float Progress = (Max > 0.0f) ? (static_cast<float>(Current) / static_cast<float>(Max)) : 0.0f;
	ValueProgress->SetPercent(Progress);

	if (IsValid(ValueText))
	{
		ValueText->SetText(FText::Format(
			FText::FromString("{0} / {1}"),
			FText::AsNumber(Current),
			FText::AsNumber(Max)
		));
	}
}

void UAttributeClampedUI::UnregisterAbilitySystem()
{
	UAbilitySystemComponent* AbilitySystem = ASC.Get();
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->GetGameplayAttributeValueChangeDelegate(BaseAttribute).RemoveAll(this);
		AbilitySystem->GetGameplayAttributeValueChangeDelegate(MaxAttribute).RemoveAll(this);
	}
	ASC.Reset();
}

void UAttributeClampedUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(TitleText))
	{
		TitleText->SetText(FText::Format(FText::FromString("{0}:"), Title));
	}
}

void UAttributeClampedUI::NativeDestruct()
{
	UnregisterAbilitySystem();
	Super::NativeDestruct();
}

