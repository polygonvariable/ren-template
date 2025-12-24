// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AttributeScalarWidget.h"

// Engine Headers
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemInterface.h"

#include "Components/PanelWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

// Project Headers
#include "RenAbility/Public/Component/RAbilitySystemComponent.h"
#include "RCoreLibrary/Public/LogMacro.h"



void UAttributeScalarWidget::RegisterASC(URAbilitySystemComponent* TargetASC)
{
	CleanUpASC();

	if (!IsValid(TargetASC))
	{
		LOG_ERROR(LogTemp, TEXT("AbilitySystemComponent is not valid"));
		return;
	}

	ASC = TargetASC;

	CurrentValue = ASC->GetAggregatedNumericAttribute(BaseAttribute);
	HandleValueChanged();

	ASC->GetGameplayAttributeValueChangeDelegate(BaseAttribute).AddWeakLambda(this, [&](const FOnAttributeChangeData& Data) { OnAggregatedRefresh(); });
	ASC->OnAggregatedRefresh.AddDynamic(this, &UAttributeScalarWidget::OnAggregatedRefresh);
}

void UAttributeScalarWidget::OnAggregatedRefresh()
{
	if (!ASC.IsValid())
	{
		LOG_ERROR(LogTemp, TEXT("AbilitySystemComponent is not valid"));
		return;
	}

	CurrentValue = ASC->GetAggregatedNumericAttribute(BaseAttribute);
	HandleValueChanged();
}

void UAttributeScalarWidget::HandleValueChanged_Implementation()
{
	if (IsValid(ValueTextBlock))
	{
		ValueTextBlock->SetText(FText::AsNumber(CurrentValue));
	}
}

void UAttributeScalarWidget::CleanUpASC()
{
	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(BaseAttribute).RemoveAll(this);
		ASC->OnAggregatedRefresh.RemoveAll(this);
		ASC.Reset();
	}
}

void UAttributeScalarWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(TitleTextBlock))
	{
		TitleTextBlock->SetText(FText::Format(FText::FromString("{0}:"), Title));
	}
}

void UAttributeScalarWidget::NativeDestruct()
{
	CleanUpASC();

	Super::NativeDestruct();
}





URAbilitySystemComponent* UPlayerAttributeScalarWidget::GetASCFromPlayer(AActor* Player)
{
	if (!IsValid(Player)) return nullptr;

	URAbilitySystemComponent* PlayerASC = Player->FindComponentByClass<URAbilitySystemComponent>();
	if (!IsValid(PlayerASC)) return nullptr;

	return PlayerASC;
}

void UPlayerAttributeScalarWidget::RegisterPlayer()
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

void UPlayerAttributeScalarWidget::CleanUpPlayer()
{
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		PlayerController->GetOnNewPawnNotifier().RemoveAll(this);
	}
}

void UPlayerAttributeScalarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RegisterPlayer();
}

void UPlayerAttributeScalarWidget::NativeDestruct()
{
	CleanUpPlayer();

	Super::NativeDestruct();
}







void UAbilityDebugWidget::OnEffectRemoved(const FActiveGameplayEffect& Effect)
{
	if (!EffectCount || !ASC.IsValid()) return;

	int Count = ASC->GetNumActiveGameplayEffects();
	EffectCount->SetText(FText::AsNumber(Count));
}

void UAbilityDebugWidget::CleanUpASC()
{
	if (ASC.IsValid())
	{
		ASC->OnGameplayEffectAppliedDelegateToSelf.RemoveAll(this);
		ASC->OnGameplayEffectAppliedDelegateToTarget.RemoveAll(this);

		FOnGivenActiveGameplayEffectRemoved& Delegate = ASC->OnAnyGameplayEffectRemovedDelegate();
		Delegate.RemoveAll(this);

		ASC.Reset();
	}
}

void UAbilityDebugWidget::RegisterASC(URAbilitySystemComponent* TargetASC)
{
	CleanUpASC();

	if (!IsValid(TargetASC))
	{
		LOG_ERROR(LogTemp, TEXT("AbilitySystemComponent is not valid"));
		return;
	}

	ASC = TargetASC;

	FOnGivenActiveGameplayEffectRemoved& Delegate = ASC->OnAnyGameplayEffectRemovedDelegate();
	Delegate.AddUObject(this, &UAbilityDebugWidget::OnEffectRemoved);

	ASC->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAbilityDebugWidget::OnEffectApplied);
	ASC->OnGameplayEffectAppliedDelegateToTarget.AddUObject(this, &UAbilityDebugWidget::OnEffectApplied);
}


URAbilitySystemComponent* UAbilityDebugWidget::GetASCFromPlayer(AActor* Player)
{
	if (!IsValid(Player)) return nullptr;

	URAbilitySystemComponent* PlayerASC = Player->FindComponentByClass<URAbilitySystemComponent>();
	if (!IsValid(PlayerASC)) return nullptr;

	return PlayerASC;
}

void UAbilityDebugWidget::RegisterPlayer()
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

void UAbilityDebugWidget::CleanUpPlayer()
{
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		PlayerController->GetOnNewPawnNotifier().RemoveAll(this);
	}
}

void UAbilityDebugWidget::OnEffectApplied(UAbilitySystemComponent* InASC, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle)
{
	if (!EffectCount || !InASC) return;

	int Count = InASC->GetNumActiveGameplayEffects();
	EffectCount->SetText(FText::AsNumber(Count));
}

void UAbilityDebugWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RegisterPlayer();
}

void UAbilityDebugWidget::NativeDestruct()
{
	CleanUpPlayer();

	Super::NativeDestruct();
}
