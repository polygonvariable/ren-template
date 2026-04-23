// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/EquipmentItemUI.h"

// Engine Headers
#include "Components/PanelWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"

// Project Headers
#include "Asset/CoreDataAsset.h"
#include "Component/EquipmentManagerComponent.h"
#include "Controller/EquipmentController.h"
#include "Settings/EquipmentSettings.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Asset/EquipmentDataAsset.h"


void UEquipmentItemUI::RegisterEquipmentComponent(AActor* Target)
{
	ResetDetail();
	UnregisterEquipmentComponent();
	UnregisterEquipmentController();

	if (!IsValid(Target))
	{
		return;
	}

	EquipmentComponent = Target->FindComponentByClass<UEquipmentManagerComponent>();
	if (!IsValid(EquipmentComponent))
	{
		return;
	}

	EquipmentComponent->OnEquipmentChangeBegin.AddUObject(this, &UEquipmentItemUI::HandleEquipmentChangeBegin);
	EquipmentComponent->OnEquipmentChangeEnd.AddUObject(this, &UEquipmentItemUI::HandleEquipmentChangeEnd);
}

void UEquipmentItemUI::UnregisterEquipmentComponent()
{
	if (IsValid(EquipmentComponent))
	{
		EquipmentComponent->OnEquipmentChangeBegin.RemoveAll(this);
		EquipmentComponent->OnEquipmentChangeEnd.RemoveAll(this);
	}
	EquipmentComponent = nullptr;
}

void UEquipmentItemUI::RegisterEquipmentController()
{
	if (!IsValid(EquipmentComponent))
	{
		return;
	}

	EquipmentController = EquipmentComponent->GetEquipmentControllerByTag(EquipmentSlot);
}

void UEquipmentItemUI::UnregisterEquipmentController()
{
	EquipmentController = nullptr;
	ResetDetail();
}

void UEquipmentItemUI::HandleEquipmentChangeBegin()
{
	ResetDetail();
	UnregisterEquipmentController();
}

void UEquipmentItemUI::HandleEquipmentChangeEnd()
{
	RegisterEquipmentController();
	SetDetail();
}

void UEquipmentItemUI::SetDetail()
{
	if (!IsValid(EquipmentController))
	{
		return;
	}

	EquipmentImage->SetBrushFromSoftTexture(EquipmentController->EquipmentAsset->Icon);
}

void UEquipmentItemUI::ResetDetail()
{
}

void UEquipmentItemUI::RegisterPlayer()
{
	APlayerController* Controller = GetOwningPlayer();
	if (!IsValid(Controller))
	{
		return;
	}

	Controller->GetOnNewPawnNotifier().AddWeakLambda(this,
		[&](APawn* NewPawn)
		{
			RegisterEquipmentComponent(NewPawn);
		}
	);

	APawn* ExistingPawn = Controller->GetPawn();
	if (IsValid(ExistingPawn))
	{
		RegisterEquipmentComponent(ExistingPawn);
	}
}

void UEquipmentItemUI::CleanUpPlayer()
{
	APlayerController* Controller = GetOwningPlayer();
	if (IsValid(Controller))
	{
		Controller->GetOnNewPawnNotifier().RemoveAll(this);
	}
}

void UEquipmentItemUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	KeyTextBlock->SetText(KeyText);
}

void UEquipmentItemUI::NativeConstruct()
{
	RegisterPlayer();

	Super::NativeConstruct();
}

void UEquipmentItemUI::NativeDestruct()
{
	CleanUpPlayer();
	UnregisterEquipmentComponent();
	UnregisterEquipmentController();

	Super::NativeDestruct();
}

void UEquipmentItemUI::OnControllerRemoved_Implementation()
{
}
void UEquipmentItemUI::OnControllerAdded_Implementation()
{
}






void UEquipmentWeaponItemUI::RegisterEquipmentController()
{
	Super::RegisterEquipmentController();

	UEquipmentController_Weapon* Weapon = Cast<UEquipmentController_Weapon>(EquipmentController);
	if (!IsValid(Weapon))
	{
		return;
	}

	OnControllerAdded();
	RefreshDetail(Weapon->GetProjectileCount(), Weapon->GetMaxProjectileCount());
	Weapon->OnEquipmentDataChanged.AddUObject(this, &UEquipmentWeaponItemUI::RefreshDetail);
}

void UEquipmentWeaponItemUI::UnregisterEquipmentController()
{
	UEquipmentController_Weapon* Weapon = Cast<UEquipmentController_Weapon>(EquipmentController);
	if (IsValid(Weapon))
	{
		Weapon->OnEquipmentDataChanged.RemoveAll(this);
	}
	Super::UnregisterEquipmentController();
}

void UEquipmentWeaponItemUI::RefreshDetail(int Current, int Max)
{
	CurrentTextBlock->SetText(FText::AsNumber(Current));
	MaxTextBlock->SetText(FText::AsNumber(Max));
}

void UEquipmentWeaponItemUI::ResetDetail()
{
	CurrentTextBlock->SetText(FText::FromString("--"));
	MaxTextBlock->SetText(FText::FromString("--"));
	OnControllerRemoved();
}








void UEquipmentSkillItemUI::RegisterEquipmentComponent(AActor* Target)
{
	FormatOptions.MinimumIntegralDigits = 1;
	FormatOptions.MinimumFractionalDigits = 1;
	FormatOptions.MaximumFractionalDigits = 1;

	Super::RegisterEquipmentComponent(Target);
	RegisterAbilitySystem(Target);
}

void UEquipmentSkillItemUI::UnregisterEquipmentComponent()
{
	Super::UnregisterEquipmentComponent();
	UnregisterAbilitySystem();
}

void UEquipmentSkillItemUI::RegisterAbilitySystem(AActor* Target)
{
	if (!IsValid(Target))
	{
		return;
	}

	AbilitySystemComponent = Target->FindComponentByClass<UAbilitySystemComponent>();
	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}

	AbilitySystemComponent->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UEquipmentSkillItemUI::HandleGameplayEffectApplied);
	AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().AddWeakLambda(this,
		[this](const FActiveGameplayEffect& Effect)
		{
			if (Effect.Spec.DynamicGrantedTags.HasTag(EffectTag))
			{
				ResetDetail();
				CleanUpTimer();
			}
		}
	);
}

void UEquipmentSkillItemUI::UnregisterAbilitySystem()
{
	CleanUpTimer();

	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);
		AbilitySystemComponent->OnGameplayEffectAppliedDelegateToSelf.RemoveAll(this);
	}
	AbilitySystemComponent = nullptr;
}

void UEquipmentSkillItemUI::HandleGameplayEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (Spec.DynamicGrantedTags.HasTag(EffectTag))
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
			TimerManager.SetTimer(TimerHandle, this, &UEquipmentSkillItemUI::HandleEffectTimeChanged, 0.1f, FTimerManagerTimerParameters{ .bLoop = true, .bMaxOncePerFrame = true });
			PRINT_WARNING(LogTemp, 1.0f, TEXT("New Tag Timer Created"));
		}
	}
}

void UEquipmentSkillItemUI::HandleEffectTimeChanged()
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

		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		if (TimerManager.IsTimerActive(TimerHandle))
		{
			TimerManager.PauseTimer(TimerHandle);
		}

		PRINT_WARNING(LogTemp, 1.0f, TEXT("Timer paused"));
	}
}

void UEquipmentSkillItemUI::GetEffectDurationAndRemainingTime(float& Duration, float& RemainingTime)
{
	Duration = 0.0f;
	RemainingTime = 0.0f;

	if (IsValid(AbilitySystemComponent))
	{
		FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAllOwningTags(FGameplayTagContainer(EffectTag));
		TArray<TPair<float, float>> TimeRemainingAndDuration = AbilitySystemComponent->GetActiveEffectsTimeRemainingAndDuration(Query);

		float LongestRemaining = 0.0f;
		float LongestDuration = 0.0f;

		for (const TPair<float, float>& Kv : TimeRemainingAndDuration)
		{
			if (Kv.Key > LongestRemaining)
			{
				LongestRemaining = Kv.Key;
				LongestDuration = Kv.Value;
			}
		}

		Duration = LongestDuration;
		RemainingTime = LongestRemaining;
	}
}

void UEquipmentSkillItemUI::CleanUpTimer()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearAllTimersForObject(this);
	TimerHandle.Invalidate();
}

void UEquipmentSkillItemUI::SetDetail()
{
	Super::SetDetail();

	if (!IsValid(EquipmentController))
	{
		ResetDetail();
		return;
	}

	const UEquipmentAbilityCollection* AbilityCollection = EquipmentController->GetEquipmentAbilityCollection();
	if (!IsValid(AbilityCollection))
	{
		ResetDetail();
		return;
	}

	const TArray<TSubclassOf<UGameplayAbility>>& Abilities = AbilityCollection->AbilityClasses;
	if (!Abilities.IsValidIndex(0) || !IsValid(Abilities[0]))
	{
		ResetDetail();
		return;
	}

	const UGameplayAbility* AbilityCDO = Abilities[0]->GetDefaultObject<UGameplayAbility>();
	const UGameplayEffect* EffectCDO = AbilityCDO->GetCostGameplayEffect();

	if (!IsValid(EffectCDO) || !EffectCDO->Modifiers.IsValidIndex(0))
	{
		ResetDetail();
		return;
	}

	float Cost = 0.0f;
	if (!EffectCDO->Modifiers[0].ModifierMagnitude.GetStaticMagnitudeIfPossible(1, Cost))
	{
		ResetDetail();
		return;
	}

	CostTextBlock->SetText(FText::AsNumber(FMath::Abs(Cost)));
	OnControllerAdded();
}

void UEquipmentSkillItemUI::ResetDetail()
{
	CooldownProgressBar->SetPercent(0.0f);

	CooldownTextBlock->SetText(FText::AsNumber(0.0f, &FormatOptions));
	CooldownTextBlock->SetVisibility(ESlateVisibility::Collapsed);

	if (!IsValid(EquipmentController))
	{
		OnControllerRemoved();
		CostTextBlock->SetText(FText::AsNumber(0));
	}
	CostTextBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
}
