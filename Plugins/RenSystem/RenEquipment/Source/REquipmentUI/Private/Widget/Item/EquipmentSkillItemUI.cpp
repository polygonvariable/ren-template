// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/Item/EquipmentSkillItemUI.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Core/EquipmentSettings.h"
#include "Data/EquipmentAbilityCollection.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "System/EquipmentController.h"


UAbilitySystemComponent* UEquipmentSkillItemUI::GetAbilitySystemComponent() const
{
	return _AbilitySystemComponent.Get();
}


void UEquipmentSkillItemUI::OnPlayerRegistered(AActor* Target)
{
	Super::OnPlayerRegistered(Target);
	RegisterAbilitySystem(Target);
}

void UEquipmentSkillItemUI::OnPlayerUnregistered()
{
	UnregisterAbilitySystem();
	Super::OnPlayerUnregistered();
}


void UEquipmentSkillItemUI::RegisterAbilitySystem(AActor* Target)
{
	if (!IsValid(Target))
	{
		return;
	}

	UAbilitySystemComponent* ASC = Target->FindComponentByClass<UAbilitySystemComponent>();
	if (!IsValid(ASC))
	{
		return;
	}

	ASC->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UEquipmentSkillItemUI::HandleOnGameplayEffectApplied);
	ASC->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &UEquipmentSkillItemUI::HandleOnGameplayEffectRemoved);

	_AbilitySystemComponent = TWeakObjectPtr<UAbilitySystemComponent>(ASC);
}

void UEquipmentSkillItemUI::UnregisterAbilitySystem()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (IsValid(ASC))
	{
		ASC->OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);
		ASC->OnGameplayEffectAppliedDelegateToSelf.RemoveAll(this);
	}
	_AbilitySystemComponent.Reset();
}


void UEquipmentSkillItemUI::HandleOnGameplayEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle)
{
	UEquipmentController* Controller = Cast<UEquipmentController>(Spec.GetContext().GetSourceObject());
	if (!IsValid(Controller))
	{
		LOG_ERROR(LogEquipment, TEXT("Gameplay effect source is not of type equipment controller"));
	}

	const UEquipmentSettings* Settings = UEquipmentSettings::Get();
	if (Controller == GetEquipmentController() && Spec.GetDynamicAssetTags().HasTagExact(Settings->EquipmentCooldownTag))
	{
		CreateCooldownCache();

		UWorld* World = GetWorld();
		FTimerManager& TimerManager = World->GetTimerManager();

		if (TimerHandle.IsValid())
		{
			if (TimerManager.IsTimerPaused(TimerHandle))
			{
				TimerManager.UnPauseTimer(TimerHandle);
				PRINT_WARNING(LogEquipment, 1.0f, TEXT("Timer was not active now restarting"));
			}
		}
		else
		{
			TimerManager.SetTimer(TimerHandle, this, &UEquipmentSkillItemUI::HandleOnEffectTimeChanged, 0.1f, FTimerManagerTimerParameters{ .bLoop = true, .bMaxOncePerFrame = true });
			PRINT_WARNING(LogEquipment, 1.0f, TEXT("New Tag Timer Created"));
		}
	}
}

void UEquipmentSkillItemUI::HandleOnGameplayEffectRemoved(const FActiveGameplayEffect& Effect)
{
	UEquipmentController* Controller = Cast<UEquipmentController>(Effect.Spec.GetContext().GetSourceObject());
	if (!IsValid(Controller))
	{
		LOG_ERROR(LogEquipment, TEXT("Gameplay effect source is not of type equipment controller"));
	}

	const UEquipmentSettings* Settings = UEquipmentSettings::Get();
	if (Controller == GetEquipmentController() && Effect.Spec.GetDynamicAssetTags().HasTagExact(Settings->EquipmentCooldownTag))
	{
		CleanUpTimer();
		ClearCooldownCache();

		CooldownProgressBar->SetPercent(0.0f);
		CooldownTextBlock->SetText(FText::AsNumber(0.0f, &FormatOptions));

		CooldownTextBlock->SetVisibility(ESlateVisibility::Collapsed);
		CostTextBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UEquipmentSkillItemUI::HandleOnEffectTimeChanged()
{
	float Duration = 0.0f;
	float RemainingTime = 0.0f;

	GetEffectDurationAndRemainingTime(Duration, RemainingTime);

	CooldownProgressBar->SetPercent(RemainingTime > 0.0f ? (RemainingTime / Duration) : 0.0f);
	CooldownTextBlock->SetText(FText::AsNumber(RemainingTime, &FormatOptions));

	CooldownTextBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
	CostTextBlock->SetVisibility(ESlateVisibility::Collapsed);

	if (FMath::IsNearlyZero(RemainingTime, 0.1f))
	{
		CooldownProgressBar->SetPercent(0.0f);
		CooldownTextBlock->SetText(FText::AsNumber(0.0f, &FormatOptions));

		CooldownTextBlock->SetVisibility(ESlateVisibility::Collapsed);
		CostTextBlock->SetVisibility(ESlateVisibility::HitTestInvisible);

		UWorld* World = GetWorld();
		FTimerManager& TimerManager = World->GetTimerManager();
		if (TimerManager.IsTimerActive(TimerHandle))
		{
			TimerManager.PauseTimer(TimerHandle);
		}

		PRINT_WARNING(LogEquipment, 1.0f, TEXT("Timer paused"));
	}
}


void UEquipmentSkillItemUI::CreateCooldownCache()
{
	ClearCooldownCache();

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (IsValid(ASC))
	{
		const UEquipmentSettings* Settings = UEquipmentSettings::Get();
		UEquipmentController* Controller = GetEquipmentController();

		TArray<FActiveGameplayEffectHandle> Handles = ASC->GetActiveEffectsWithAllTags(FGameplayTagContainer(Settings->EquipmentCooldownTag));
		for (const FActiveGameplayEffectHandle& Handle : Handles)
		{
			const FActiveGameplayEffect* Effect = ASC->GetActiveGameplayEffect(Handle);
			if (!Effect)
			{
				continue;
			}

			UObject* SourceController = Effect->Spec.GetContext().GetSourceObject();
			if (SourceController == Controller)
			{
				_CachedHandles.Add(Handle);
			}
		}
	}
}

void UEquipmentSkillItemUI::ClearCooldownCache()
{
	_CachedHandles.Empty();
}

void UEquipmentSkillItemUI::GetEffectDurationAndRemainingTime(float& Duration, float& RemainingTime)
{
	Duration = 0.0f;
	RemainingTime = 0.0f;

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (IsValid(ASC))
	{
		for (const FActiveGameplayEffectHandle& Handle : _CachedHandles)
		{
			const FActiveGameplayEffect* Effect = ASC->GetActiveGameplayEffect(Handle);
			if (!Effect)
			{
				continue;
			}

			const float CurrentTime = ASC->GetWorld()->GetTimeSeconds();
			const float CurrentRemaining = Effect->GetTimeRemaining(CurrentTime);

			if (CurrentRemaining > RemainingTime)
			{
				RemainingTime = CurrentRemaining;
				Duration = Effect->GetDuration();
			}
		}
	}
}

void UEquipmentSkillItemUI::CleanUpTimer()
{
	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();

	TimerManager.ClearAllTimersForObject(this);
	TimerHandle.Invalidate();
}


void UEquipmentSkillItemUI::SetDetail(UEquipmentController* Controller)
{
	Super::SetDetail(Controller);

	if (!IsValid(Controller))
	{
		ResetDetail();
		return;
	}

	const UEquipmentAbilityCollection* AbilityCollection = Controller->GetEquipmentAbilityCollection();
	if (!IsValid(AbilityCollection))
	{
		ResetDetail();
		return;
	}

	const TArray<FEquipmentAbilityData>& Abilities = AbilityCollection->Abilities;
	if (Abilities.IsValidIndex(0) && IsValid(Abilities[0].AbilityClass))
	{
		const UGameplayAbility* AbilityCDO = Abilities[0].AbilityClass->GetDefaultObject<UGameplayAbility>();
		const UGameplayEffect* EffectCDO = AbilityCDO->GetCostGameplayEffect();

		if (!IsValid(EffectCDO) || !EffectCDO->Modifiers.IsValidIndex(0))
		{
			return;
		}

		float Cost = 0.0f;
		if (!EffectCDO->Modifiers[0].ModifierMagnitude.GetStaticMagnitudeIfPossible(1, Cost))
		{
			return;
		}

		CostTextBlock->SetText(FText::AsNumber(FMath::Abs(Cost)));
	}
}

void UEquipmentSkillItemUI::ResetDetail()
{
	Super::ResetDetail();

	CooldownProgressBar->SetPercent(0.0f);

	CooldownTextBlock->SetText(FText::AsNumber(0.0f, &FormatOptions));
	CooldownTextBlock->SetVisibility(ESlateVisibility::Collapsed);

	CostTextBlock->SetText(FText::AsNumber(0));
	CostTextBlock->SetVisibility(ESlateVisibility::HitTestInvisible);

	CleanUpTimer();
	ClearCooldownCache();
}

void UEquipmentSkillItemUI::NativeConstruct()
{
	FormatOptions.MinimumIntegralDigits = 1;
	FormatOptions.MinimumFractionalDigits = 1;
	FormatOptions.MaximumFractionalDigits = 1;

	Super::NativeConstruct();
}

