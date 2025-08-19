// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/EffectCollectionWidget.h"

// Engine Headers
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"

// Project Headers
#include "RenAbility/Public/Component/RAbilitySystemComponent.h"
#include "RenAbility/Public/Widget/EffectCollectionEntryWidget.h"
#include "RenCoreLibrary/Public/LogMacro.h"



void UEffectCollectionWidget::RegisterASC(UAbilitySystemComponent* TargetASC)
{
	CleanUpASC();

	if (!IsValid(TargetASC))
	{
		LOG_ERROR(LogTemp, TEXT("AbilitySystemComponent is not valid"));
		return;
	}

	ASC = TargetASC;

	ASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &UEffectCollectionWidget::OnEffectAdded);
	ASC->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &UEffectCollectionWidget::OnEffectRemoved);
}

void UEffectCollectionWidget::OnEffectAdded(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	if (!AssetTags.HasTagExact(CaptureEffectTag))
	{
		return;
	}

	HandleEffectAdded(SpecApplied, ActiveHandle);
}

void UEffectCollectionWidget::OnEffectRemoved(const FActiveGameplayEffect& EffectRemoved)
{
	FGameplayTagContainer AssetTags;
	EffectRemoved.Spec.GetAllAssetTags(AssetTags);

	if (!AssetTags.HasTagExact(CaptureEffectTag))
	{
		return;
	}

	HandleEffectRemoved(EffectRemoved);
}

void UEffectCollectionWidget::HandleEffectAdded_Implementation(const FGameplayEffectSpec& SpecApplied, const FActiveGameplayEffectHandle& ActiveHandle)
{
	if (IsValid(EffectStackPanel) && IsValid(EntryWidgetClass) && ASC.IsValid())
	{
		UEffectCollectionEntryWidget* EntryWidget = CreateWidget<UEffectCollectionEntryWidget>(this, EntryWidgetClass);
		if (IsValid(EntryWidget))
		{
			EntryWidget->RegisterASC(ASC.Get(), SpecApplied, ActiveHandle);
			EffectStackPanel->AddChild(EntryWidget);
		}
	}
}

void UEffectCollectionWidget::HandleEffectRemoved_Implementation(const FActiveGameplayEffect& EffectRemoved)
{

}

void UEffectCollectionWidget::CleanUpASC()
{
	if (ASC.IsValid())
	{
		ASC->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
		ASC->OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);
		ASC.Reset();
	}
}

void UEffectCollectionWidget::NativeDestruct()
{
	CleanUpASC();

	Super::NativeDestruct();
}





UAbilitySystemComponent* UPlayerEffectCollectionWidget::GetASCFromPlayer(AActor* Player)
{
	if (!IsValid(Player)) return nullptr;

	UAbilitySystemComponent* PlayerASC = Player->FindComponentByClass<UAbilitySystemComponent>();
	if (!IsValid(PlayerASC)) return nullptr;

	return PlayerASC;
}

void UPlayerEffectCollectionWidget::RegisterPlayer()
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

void UPlayerEffectCollectionWidget::CleanUpPlayer()
{
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		PlayerController->GetOnNewPawnNotifier().RemoveAll(this);
	}
}

void UPlayerEffectCollectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RegisterPlayer();
}

void UPlayerEffectCollectionWidget::NativeDestruct()
{
	CleanUpPlayer();

	Super::NativeDestruct();
}
