// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AttributeClampedWidget.h"

// Engine Headers
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"

// Project Headers
#include "RenAbility/Public/Component/RAbilitySystemComponent.h"
#include "RenCore/Public/Macro/LogMacro.h"



void UAttributeClampedWidget::RegisterASC(URAbilitySystemComponent* TargetASC)
{
	CleanUpASC();

	if (!IsValid(TargetASC))
	{
		LOG_ERROR(LogTemp, TEXT("AbilitySystemComponent is not valid"));
		return;
	}

	ASC = TargetASC;

	CurrentValue = ASC->GetAggregatedNumericAttribute(BaseAttribute);
	CurrentMax = ASC->GetAggregatedNumericAttribute(MaxAttribute);
	HandleValueChanged();

	ASC->GetGameplayAttributeValueChangeDelegate(BaseAttribute).AddWeakLambda(this, [&](const FOnAttributeChangeData& Data) { OnAggregatedRefresh(); });
	ASC->GetGameplayAttributeValueChangeDelegate(MaxAttribute).AddWeakLambda(this, [&](const FOnAttributeChangeData& Data) { OnAggregatedRefresh(); });
	ASC->OnAggregatedRefresh.AddDynamic(this, &UAttributeClampedWidget::OnAggregatedRefresh);
}

void UAttributeClampedWidget::OnAggregatedRefresh()
{
	if (!ASC.IsValid())
	{
		LOG_ERROR(LogTemp, TEXT("AbilitySystemComponent is not valid"));
		return;
	}

	CurrentValue = ASC->GetAggregatedNumericAttribute(BaseAttribute);
	CurrentMax = ASC->GetAggregatedNumericAttribute(MaxAttribute);
	HandleValueChanged();
}

void UAttributeClampedWidget::HandleValueChanged_Implementation()
{
	if (IsValid(ValueProgressBar))
	{
		ValueProgressBar->SetPercent(CurrentMax > 0.0f ? (CurrentValue / CurrentMax) : 0.0f);
	}

	if (IsValid(ValueTextBlock))
	{
		FText FormattedText = FText::Format(
			FText::FromString("{0} / {1}"),
			FText::AsNumber(CurrentValue),
			FText::AsNumber(CurrentMax)
		);

		ValueTextBlock->SetText(FormattedText);
	}
}

void UAttributeClampedWidget::CleanUpASC()
{
	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(BaseAttribute).RemoveAll(this);
		ASC->OnAggregatedRefresh.RemoveAll(this);
		ASC.Reset();
	}
}


void UAttributeClampedWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(TitleTextBlock))
	{
		TitleTextBlock->SetText(FText::Format(FText::FromString("{0}:"), Title));
	}
}





void UAttributeClampedWidget::NativeDestruct()
{
	CleanUpASC();

	Super::NativeDestruct();
}

URAbilitySystemComponent* UPlayerAttributeClampedWidget::GetASCFromPlayer(AActor* Player)
{
	if (!IsValid(Player)) return nullptr;

	URAbilitySystemComponent* PlayerASC = Player->FindComponentByClass<URAbilitySystemComponent>();
	if (!IsValid(PlayerASC)) return nullptr;

	return PlayerASC;
}

void UPlayerAttributeClampedWidget::RegisterPlayer()
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

void UPlayerAttributeClampedWidget::CleanUpPlayer()
{
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		PlayerController->GetOnNewPawnNotifier().RemoveAll(this);
	}
}

void UPlayerAttributeClampedWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RegisterPlayer();
}

void UPlayerAttributeClampedWidget::NativeDestruct()
{
	CleanUpPlayer();

	Super::NativeDestruct();
}

