// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"
#include "Components/ProgressBar.h"

// Generated Headers
#include "EquipmentItemUI.generated.h"

// Forward Declarations
class UImage;
class UTextBlock;
class UProgressBar;
class UAbilitySystemComponent;
class UEquipmentManagerComponent;
class UEquipmentController;
struct FGameplayEffectSpec;
struct FActiveGameplayEffect;


/**
 *
 */
UCLASS(Abstract)
class UEquipmentItemUI : public UUserWidget
{

	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> EquipmentImage = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> KeyTextBlock;

	UPROPERTY(EditAnywhere)
	FText KeyText;

	UPROPERTY(EditAnywhere)
	FGameplayTag EquipmentSlot;


	UEquipmentManagerComponent* GetEquipmentComponent() const;
	UEquipmentController* GetEquipmentController() const;

	template<class T>
	T* GetEquipmentComponent() const
	{
		return Cast<T>(_EquipmentComponent.Get());
	}

	template<class T>
	T* GetEquipmentController() const
	{
		return Cast<T>(_EquipmentController.Get());
	}

	virtual void RegisterEquipmentComponent(AActor* Target);
	virtual void UnregisterEquipmentComponent();

	virtual void RegisterEquipmentController();
	virtual void UnregisterEquipmentController();

	virtual void SetDetail(UEquipmentController* Controller);
	virtual void RefreshDetail();
	virtual void ResetDetail();

	void RegisterPlayer();
	void UnregisterPlayer();

	virtual void OnPlayerRegistered(AActor* Target);
	virtual void OnPlayerUnregistered();

	// ~ UUserWidget
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

private:

	UPROPERTY()
	TWeakObjectPtr<UEquipmentManagerComponent> _EquipmentComponent = nullptr;

	UPROPERTY()
	TWeakObjectPtr<UEquipmentController>  _EquipmentController = nullptr;

};








/**
 *
 */
UCLASS(Abstract)
class UEquipmentWeaponItemUI : public UEquipmentItemUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxTextBlock;

	void UpdateWeaponData();

	// ~ UEquipmentItemUI
	virtual void RegisterEquipmentController() override;
	virtual void UnregisterEquipmentController() override;

	virtual void SetDetail(UEquipmentController* Controller) override;
	virtual void RefreshDetail() override;
	virtual void ResetDetail() override;
	// ~ End of UEquipmentItemUI

};











/**
 *
 */
UCLASS(Abstract)
class UEquipmentSkillItemUI : public UEquipmentItemUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> CooldownProgressBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CooldownTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CostTextBlock;

	UPROPERTY(EditAnywhere)
	FGameplayTag EffectTag;

	FNumberFormattingOptions FormatOptions;
	FTimerHandle TimerHandle;


	UAbilitySystemComponent* GetAbilitySystemComponent() const;

	// ~ UEquipmentItemUI
	virtual void OnPlayerRegistered(AActor* Target) override;
	virtual void OnPlayerUnregistered() override;
	// ~ End of UEquipmentItemUI

	void RegisterAbilitySystem(AActor* Target);
	void UnregisterAbilitySystem();

	void HandleGameplayEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle);
	void HandleGameplayEffectRemoved(const FActiveGameplayEffect& Effect);
	void HandleEffectTimeChanged();

	void GetEffectDurationAndRemainingTime(float& Duration, float& RemainingTime);
	void CleanUpTimer();

	// ~ UEquipmentItemUI
	virtual void SetDetail(UEquipmentController* Controller) override;
	virtual void ResetDetail() override;
	virtual void NativeConstruct() override;
	// ~ End of UEquipmentItemUI

private:

	UPROPERTY()
	TWeakObjectPtr<UAbilitySystemComponent> _AbilitySystemComponent = nullptr;

};

