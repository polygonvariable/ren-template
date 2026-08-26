// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"

// Project Headers
#include "Widget/EquipmentItemUI.h"

// Generated Headers
#include "EquipmentSkillItemUI.generated.h"

// Forward Declarations
class UProgressBar;
class UAbilitySystemComponent;
struct FGameplayEffectSpec;
struct FActiveGameplayEffect;


/**
 *
 */
UCLASS(Abstract)
class UEquipmentSkillItemUI : public UEquipmentItemUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> CooldownProgressBar = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CooldownTextBlock = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CostTextBlock = nullptr;

	UPROPERTY(EditAnywhere)
	FGameplayTag EffectTag;

	FNumberFormattingOptions FormatOptions;
	FTimerHandle TimerHandle;


	UAbilitySystemComponent* GetAbilitySystemComponent() const;

	void RegisterAbilitySystem(AActor* Target);
	void UnregisterAbilitySystem();

	void CreateCooldownCache();
	void ClearCooldownCache();

	void GetEffectDurationAndRemainingTime(float& Duration, float& RemainingTime);
	void CleanUpTimer();

	// ~ Bindings
	void HandleOnGameplayEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle);
	void HandleOnGameplayEffectRemoved(const FActiveGameplayEffect& Effect);
	void HandleOnEffectTimeChanged();
	// ~ End of Bindings
	
	// ~ UEquipmentItemUI
	virtual void OnPlayerRegistered(AActor* Target) override;
	virtual void OnPlayerUnregistered() override;
	// ~ End of UEquipmentItemUI

	// ~ UEquipmentItemUI
	virtual void SetDetail(UEquipmentController* Controller) override;
	virtual void ResetDetail() override;
	virtual void NativeConstruct() override;
	// ~ End of UEquipmentItemUI

private:

	UPROPERTY()
	TWeakObjectPtr<UAbilitySystemComponent> _AbilitySystemComponent = nullptr;

	TArray<FActiveGameplayEffectHandle> _CachedHandles;

};

