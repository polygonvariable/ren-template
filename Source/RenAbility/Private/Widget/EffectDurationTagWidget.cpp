// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/EffectDurationTagWidget.h"

// Engine Headers
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

// Project Headers
#include "RenAbility/Public/Component/RAbilitySystemComponent.h"
#include "RenCore/Public/Macro/LogMacro.h"



void UEffectDurationTagWidget::RegisterASC(UAbilitySystemComponent* TargetASC)
{
	CleanUpASC();

	if (!IsValid(TargetASC))
	{
		LOG_ERROR(LogTemp, TEXT("AbilitySystemComponent is not valid"));
		return;
	}

	ASC = TargetASC;
	PRINT_WARNING(LogTemp, 199.0f, TEXT("ASC Registered"));
	ASC->RegisterGameplayTagEvent(CaptureDurationTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UEffectDurationTagWidget::OnTagCountChanged);
}

void UEffectDurationTagWidget::OnTagCountChanged(const FGameplayTag CooldownTag, int NewCount)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (NewCount > 0)
	{
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
			TimerManager.SetTimer(TimerHandle, this, &UEffectDurationTagWidget::HandleEffectTimeChanged, 0.1f, FTimerManagerTimerParameters{ .bLoop = true, .bMaxOncePerFrame = true });
			PRINT_WARNING(LogTemp, 1.0f, TEXT("New Tag Timer Created"));
		}
	}
	else
	{
		if (TimerManager.IsTimerActive(TimerHandle))
		{
			DurationProgressBar->SetPercent(0.0f);
			DurationTextBlock->SetText(FText::AsNumber(0.0f, &FormatOptions));

			TimerManager.PauseTimer(TimerHandle);
			PRINT_WARNING(LogTemp, 1.0f, TEXT("Timer paused as NewCount is 0"));
		}
	}
}

void UEffectDurationTagWidget::HandleEffectTimeChanged_Implementation()
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

	if (RemainingTime <= 0.0f)
	{
		DurationProgressBar->SetPercent(0.0f);
		DurationTextBlock->SetText(FText::AsNumber(0.0f, &FormatOptions));

		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		if (TimerManager.IsTimerActive(TimerHandle))
		{
			TimerManager.PauseTimer(TimerHandle);
		}

		PRINT_WARNING(LogTemp, 1.0f, TEXT("Timer paused as RemainingTime is 0"));
	}
}

void UEffectDurationTagWidget::GetEffectDurationAndRemainingTime(float& Duration, float& RemainingTime)
{
	Duration = 0.0f;
	RemainingTime = 0.0f;

	if (ASC.IsValid())
	{
		const FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(DurationTags);
		TArray<TPair<float, float>> TimeRemainingAndDuration = ASC->GetActiveEffectsTimeRemainingAndDuration(Query);

		float LongestRemaining = 0.f;
		float LongestDuration = 0.f;

		for (const TPair<float, float>& Pair : TimeRemainingAndDuration)
		{
			if (Pair.Key > LongestRemaining)
			{
				LongestRemaining = Pair.Key;
				LongestDuration = Pair.Value;
			}
		}

		Duration = LongestDuration;
		RemainingTime = LongestRemaining;
	}
}

void UEffectDurationTagWidget::CleanUpASC()
{
	if (ASC.IsValid())
	{
		ASC->RegisterGameplayTagEvent(CaptureDurationTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		ASC.Reset();
	}
}

void UEffectDurationTagWidget::CleanUpTimer()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearAllTimersForObject(this);
	TimerHandle.Invalidate();
}

void UEffectDurationTagWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FormatOptions.MinimumIntegralDigits = 1;
	FormatOptions.MinimumFractionalDigits = 1;
	FormatOptions.MaximumFractionalDigits = 1;

	DurationTags.AddTag(CaptureDurationTag);
}

void UEffectDurationTagWidget::NativeDestruct()
{
	CleanUpASC();
	CleanUpTimer();

	Super::NativeDestruct();
}





UAbilitySystemComponent* UPlayerEffectDurationTagWidget::GetASCFromPlayer(AActor* Player)
{
	if (!IsValid(Player)) return nullptr;

	UAbilitySystemComponent* PlayerASC = Player->FindComponentByClass<UAbilitySystemComponent>();
	if (!IsValid(PlayerASC)) return nullptr;

	return PlayerASC;
}

void UPlayerEffectDurationTagWidget::RegisterPlayer()
{
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		PlayerController->GetOnNewPawnNotifier().AddWeakLambda(this, [&](APawn* NewPawn) { RegisterASC(GetASCFromPlayer(NewPawn)); });

		if (APawn* ExistingPawn = PlayerController->GetPawn())
		{
			RegisterASC(GetASCFromPlayer(ExistingPawn));
		}
	}
	else
	{
		LOG_ERROR(LogTemp, TEXT("PlayerController is not valid"));
	}
}

void UPlayerEffectDurationTagWidget::CleanUpPlayer()
{
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		PlayerController->GetOnNewPawnNotifier().RemoveAll(this);
	}
}

void UPlayerEffectDurationTagWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RegisterPlayer();
}

void UPlayerEffectDurationTagWidget::NativeDestruct()
{
	CleanUpPlayer();

	Super::NativeDestruct();
}
