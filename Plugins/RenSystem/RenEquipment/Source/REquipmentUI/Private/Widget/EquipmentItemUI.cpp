// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/EquipmentItemUI.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

// Project Headers
#include "Asset/CoreDataAsset.h"
#include "Asset/EquipmentDataAsset.h"
#include "Component/EquipmentManagerComponent.h"
#include "Controller/EquipmentController.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Settings/EquipmentSettings.h"


UEquipmentManagerComponent* UEquipmentItemUI::GetEquipmentComponent() const
{
	return _EquipmentComponent.Get();
}

UEquipmentController* UEquipmentItemUI::GetEquipmentController() const
{
	return _EquipmentController.Get();
}

void UEquipmentItemUI::RegisterEquipmentComponent(AActor* Target)
{
	if (IsValid(Target))
	{
		UEquipmentManagerComponent* Component = Target->FindComponentByClass<UEquipmentManagerComponent>();
		if (!IsValid(Component))
		{
			return;
		}

		Component->OnEquipmentSpawnBegin.AddUObject(this, &UEquipmentItemUI::UnregisterEquipmentController);
		Component->OnEquipmentSpawnEnd.AddUObject(this, &UEquipmentItemUI::RegisterEquipmentController);

		_EquipmentComponent = TWeakObjectPtr<UEquipmentManagerComponent>(Component);
	}
}

void UEquipmentItemUI::UnregisterEquipmentComponent()
{
	UEquipmentManagerComponent* Component = GetEquipmentComponent();
	if (IsValid(Component))
	{
		Component->OnEquipmentSpawnBegin.RemoveAll(this);
		Component->OnEquipmentSpawnEnd.RemoveAll(this);
	}
	_EquipmentComponent.Reset();
}

void UEquipmentItemUI::RegisterEquipmentController()
{
	UEquipmentManagerComponent* EquipmentComponent = GetEquipmentComponent();
	if (!IsValid(EquipmentComponent))
	{
		return;
	}

	UEquipmentController* Controller = EquipmentComponent->GetEquipmentControllerByTag(EquipmentSlot);
	if (!IsValid(Controller))
	{
		return;
	}

	_EquipmentController = TWeakObjectPtr<UEquipmentController>(Controller);
	SetDetail(Controller);
}

void UEquipmentItemUI::UnregisterEquipmentController()
{
	ResetDetail();
	_EquipmentController.Reset();
}

void UEquipmentItemUI::SetDetail(UEquipmentController* Controller)
{
	EquipmentImage->SetBrushFromSoftTexture(Controller->EquipmentAsset->Icon);
}

void UEquipmentItemUI::RefreshDetail() {}
void UEquipmentItemUI::ResetDetail() {}


void UEquipmentItemUI::RegisterPlayer()
{
	APlayerController* Controller = GetOwningPlayer();
	if (!IsValid(Controller))
	{
		return;
	}

	TWeakObjectPtr<UEquipmentItemUI> WeakThis(this);
	Controller->GetOnNewPawnNotifier().AddWeakLambda(this,
		[WeakThis](APawn* NewPawn)
		{
			UEquipmentItemUI* This = WeakThis.Get();
			if (IsValid(This))
			{
				This->OnPlayerRegistered(NewPawn);
			}
		}
	);

	APawn* ExistingPawn = Controller->GetPawn();
	if (IsValid(ExistingPawn))
	{
		OnPlayerRegistered(ExistingPawn);
	}
}

void UEquipmentItemUI::UnregisterPlayer()
{
	APlayerController* Controller = GetOwningPlayer();
	if (IsValid(Controller))
	{
		Controller->GetOnNewPawnNotifier().RemoveAll(this);
	}

	OnPlayerUnregistered();
}

void UEquipmentItemUI::OnPlayerRegistered(AActor* Target)
{
	OnPlayerUnregistered();

	if (IsValid(Target))
	{
		RegisterEquipmentComponent(Target);
	}
}

void UEquipmentItemUI::OnPlayerUnregistered()
{
	ResetDetail();
	UnregisterEquipmentComponent();
	UnregisterEquipmentController();
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
	UnregisterPlayer();
	Super::NativeDestruct();
}






void UEquipmentWeaponItemUI::RegisterEquipmentController()
{
	Super::RegisterEquipmentController();

	UEquipmentProjectileWeaponController* Weapon = GetEquipmentController<UEquipmentProjectileWeaponController>();
	if (IsValid(Weapon))
	{
		Weapon->OnDataChanged.AddUObject(this, &UEquipmentWeaponItemUI::RefreshDetail);
	}
}

void UEquipmentWeaponItemUI::UnregisterEquipmentController()
{
	UEquipmentProjectileWeaponController* Weapon = GetEquipmentController<UEquipmentProjectileWeaponController>();
	if (IsValid(Weapon))
	{
		Weapon->OnDataChanged.RemoveAll(this);
	}

	Super::UnregisterEquipmentController();
}

void UEquipmentWeaponItemUI::SetDetail(UEquipmentController* Controller)
{
	Super::SetDetail(Controller);
	UpdateWeaponData();
}

void UEquipmentWeaponItemUI::RefreshDetail()
{
	UpdateWeaponData();
}

void UEquipmentWeaponItemUI::ResetDetail()
{
	CurrentTextBlock->SetText(FText::FromString("--"));
	MaxTextBlock->SetText(FText::FromString("--"));
}

void UEquipmentWeaponItemUI::UpdateWeaponData()
{
	UEquipmentProjectileWeaponController* Weapon = GetEquipmentController<UEquipmentProjectileWeaponController>();
	if (IsValid(Weapon))
	{
		CurrentTextBlock->SetText(FText::AsNumber(Weapon->GetProjectileCount()));
		MaxTextBlock->SetText(FText::AsNumber(Weapon->GetMaxProjectileCount()));
	}
}







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

	UAbilitySystemComponent* AbilitySystemComponent = Target->FindComponentByClass<UAbilitySystemComponent>();
	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}

	AbilitySystemComponent->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UEquipmentSkillItemUI::HandleGameplayEffectApplied);
	AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &UEquipmentSkillItemUI::HandleGameplayEffectRemoved);

	_AbilitySystemComponent = TWeakObjectPtr<UAbilitySystemComponent>(AbilitySystemComponent);
}

void UEquipmentSkillItemUI::UnregisterAbilitySystem()
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();
	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);
		AbilitySystemComponent->OnGameplayEffectAppliedDelegateToSelf.RemoveAll(this);
	}
	_AbilitySystemComponent.Reset();
}


void UEquipmentSkillItemUI::HandleGameplayEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle)
{
	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();
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

void UEquipmentSkillItemUI::HandleGameplayEffectRemoved(const FActiveGameplayEffect& Effect)
{
	if (Effect.Spec.DynamicGrantedTags.HasTag(EffectTag))
	{
		CleanUpTimer();

		CooldownProgressBar->SetPercent(0.0f);
		CooldownTextBlock->SetText(FText::AsNumber(0.0f, &FormatOptions));

		CooldownTextBlock->SetVisibility(ESlateVisibility::Collapsed);
		CostTextBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
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

		UWorld* World = GetWorld();
		FTimerManager& TimerManager = World->GetTimerManager();
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

	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();
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
	if (!Abilities.IsValidIndex(0) || !IsValid(Abilities[0].AbilityClass))
	{
		ResetDetail();
		return;
	}

	const UGameplayAbility* AbilityCDO = Abilities[0].AbilityClass->GetDefaultObject<UGameplayAbility>();
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
}

void UEquipmentSkillItemUI::ResetDetail()
{
	CooldownProgressBar->SetPercent(0.0f);

	CooldownTextBlock->SetText(FText::AsNumber(0.0f, &FormatOptions));
	CooldownTextBlock->SetVisibility(ESlateVisibility::Collapsed);

	CostTextBlock->SetText(FText::AsNumber(0));
	CostTextBlock->SetVisibility(ESlateVisibility::HitTestInvisible);

	CleanUpTimer();
}

void UEquipmentSkillItemUI::NativeConstruct()
{
	FormatOptions.MinimumIntegralDigits = 1;
	FormatOptions.MinimumFractionalDigits = 1;
	FormatOptions.MaximumFractionalDigits = 1;

	Super::NativeConstruct();
}

