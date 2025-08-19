// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/EffectCollectionEntryWidget.h"

// Engine Headers
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

// Project Headers
#include "RenAbility/Public/Component/RAbilitySystemComponent.h"
#include "RenCoreLibrary/Public/LogMacro.h"



void UEffectCollectionEntryWidget::RegisterASC(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, const FActiveGameplayEffectHandle& ActiveHandle)
{
	CleanUpASC(CurrentActiveHandle);

	if (!IsValid(TargetASC) || !ActiveHandle.IsValid())
	{
		PRINT_ERROR(LogTemp, 5.0f, TEXT("ASC is not valid or ActiveHandle is not valid"));
		return;
	}

	ASC = TargetASC;

	ASC->OnGameplayEffectStackChangeDelegate(ActiveHandle)->AddUObject(this, &UEffectCollectionEntryWidget::OnEffectStackChanged);
	ASC->OnGameplayEffectRemoved_InfoDelegate(ActiveHandle)->AddUObject(this, &UEffectCollectionEntryWidget::HandleEffectRemoved);

	OnEffectStackChanged(ActiveHandle, SpecApplied.GetStackCount(), 0);

	CurrentActiveHandle = ActiveHandle;
}

void UEffectCollectionEntryWidget::OnEffectStackChanged(FActiveGameplayEffectHandle EffectHandle, int NewStackCount, int PreviousStackCount)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (TimerHandle.IsValid())
	{
		if (TimerManager.IsTimerPaused(TimerHandle))
		{
			TimerManager.UnPauseTimer(TimerHandle);
			PRINT_WARNING(LogTemp, 1.0f, TEXT("Timer was not active now restarting"));
		}
	}
	else
	{
		TimerManager.SetTimer(TimerHandle, this, &UEffectCollectionEntryWidget::HandleEffectTimeChanged, 0.1f, FTimerManagerTimerParameters{ .bLoop = true, .bMaxOncePerFrame = true });
		PRINT_WARNING(LogTemp, 1.0f, TEXT("New timer created"));
	}

	HandleEffectStackChanged(EffectHandle, NewStackCount, PreviousStackCount);
}

void UEffectCollectionEntryWidget::HandleEffectStackChanged_Implementation(FActiveGameplayEffectHandle EffectHandle, int NewStackCount, int PreviousStackCount)
{
	if (IsValid(StackTextBlock))
	{
		StackTextBlock->SetText(FText::AsNumber(NewStackCount));
	}
}

void UEffectCollectionEntryWidget::HandleEffectRemoved_Implementation(const FGameplayEffectRemovalInfo& EffectRemovalInfo)
{
	if (const FActiveGameplayEffect* ActiveEffect = EffectRemovalInfo.ActiveEffect)
	{
		CleanUpASC(ActiveEffect->Handle);
	}

	CleanUpTimer();

	this->RemoveFromParent();
}

void UEffectCollectionEntryWidget::HandleEffectTimeChanged_Implementation()
{
	float Duration = 0.0f;
	float RemainingTime = 0.0f;

	GetEffectDurationAndRemainingTime(Duration, RemainingTime);

	if (IsValid(DurationProgressBar))
	{
		DurationProgressBar->SetPercent(RemainingTime > 0.0f ? (RemainingTime / Duration) : 0.0f);
	}

	if (IsValid(DurationTextBlock))
	{
		DurationTextBlock->SetText(FText::AsNumber(RemainingTime, &FormatOptions));
	}
}

void UEffectCollectionEntryWidget::GetEffectDurationAndRemainingTime(float& Duration, float& RemainingTime)
{
	Duration = 0.0f;
	RemainingTime = 0.0f;

	if (ASC.IsValid() && CurrentActiveHandle.IsValid())
	{
		if (const FActiveGameplayEffect* ActiveEffect = ASC->GetActiveGameplayEffect(CurrentActiveHandle))
		{
			Duration = ActiveEffect->GetDuration();
			RemainingTime = ActiveEffect->GetTimeRemaining(ASC->GetWorld()->GetTimeSeconds());
		}
	}
}

void UEffectCollectionEntryWidget::CleanUpASC(const FActiveGameplayEffectHandle& EffectHandle)
{
	if (ASC.IsValid())
	{
		if (EffectHandle.IsValid())
		{
			if (FOnActiveGameplayEffectStackChange* StackDelegate = ASC->OnGameplayEffectStackChangeDelegate(EffectHandle))
			{
				StackDelegate->RemoveAll(this);
			}

			if (FOnActiveGameplayEffectRemoved_Info* EffectDelegate = ASC->OnGameplayEffectRemoved_InfoDelegate(EffectHandle))
			{
				EffectDelegate->RemoveAll(this);
			}
		}

		ASC.Reset();
	}
}

void UEffectCollectionEntryWidget::CleanUpTimer()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearAllTimersForObject(this);
	TimerHandle.Invalidate();

}

void UEffectCollectionEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FormatOptions.MinimumIntegralDigits = 1;
	FormatOptions.MinimumFractionalDigits = 1;
	FormatOptions.MaximumFractionalDigits = 1;
}

void UEffectCollectionEntryWidget::NativeDestruct()
{
	CleanUpASC(CurrentActiveHandle);
	CleanUpTimer();

	Super::NativeDestruct();
}

