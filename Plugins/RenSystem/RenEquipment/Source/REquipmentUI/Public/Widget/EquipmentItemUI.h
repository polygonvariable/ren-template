// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"

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


/**
 *
 */
UCLASS(Abstract)
class UEquipmentItemUI : public UUserWidget
{

	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UImage> EquipmentImage = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> KeyTextBlock;

	UPROPERTY(EditAnywhere)
	FText KeyText;

	UPROPERTY(EditAnywhere)
	FGameplayTag EquipmentSlot;

	UPROPERTY()
	TObjectPtr<UEquipmentManagerComponent> EquipmentComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UEquipmentController>  EquipmentController = nullptr;


	virtual void RegisterEquipmentComponent(AActor* Target);
	virtual void UnregisterEquipmentComponent();

	virtual void RegisterEquipmentController();
	virtual void UnregisterEquipmentController();

	void HandleEquipmentChangeBegin();
	void HandleEquipmentChangeEnd();

	virtual void SetDetail();
	virtual void ResetDetail();

	void RegisterPlayer();
	void CleanUpPlayer();

	// ~ UUserWidget
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget


	UFUNCTION(BlueprintNativeEvent)
	void OnControllerRemoved();
	void OnControllerRemoved_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void OnControllerAdded();
	void OnControllerAdded_Implementation();

};







/**
 *
 */
UCLASS(Abstract)
class UEquipmentWeaponItemUI : public UEquipmentItemUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentTextBlock;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxTextBlock;


	// ~ UEquipmentItemUI
	virtual void RegisterEquipmentController() override;
	virtual void UnregisterEquipmentController() override;

	virtual void RefreshDetail(int Current, int Max);
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

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UProgressBar> CooldownProgressBar;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> CooldownTextBlock;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> CostTextBlock;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere)
	FGameplayTag EffectTag;


	FNumberFormattingOptions FormatOptions;
	FTimerHandle TimerHandle;


	virtual void RegisterEquipmentComponent(AActor* Target) override;
	virtual void UnregisterEquipmentComponent() override;

	void RegisterAbilitySystem(AActor* Target);
	void UnregisterAbilitySystem();

	void HandleGameplayEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle);
	void HandleEffectTimeChanged();
	void GetEffectDurationAndRemainingTime(float& Duration, float& RemainingTime);

	void CleanUpTimer();

	virtual void SetDetail() override;
	virtual void ResetDetail() override;

};

