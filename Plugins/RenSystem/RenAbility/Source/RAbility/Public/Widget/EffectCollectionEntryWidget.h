// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

#include "ActiveGameplayEffectHandle.h"
#include "AttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"

// Project Headers

// Generated Headers
#include "EffectCollectionEntryWidget.generated.h"

// Forward Declarations
class UTextBlock;
class UProgressBar;



/**
 *
 */
UCLASS(Abstract)
class UEffectCollectionEntryWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	UFUNCTION()
	void RegisterASC(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, const FActiveGameplayEffectHandle& ActiveHandle);

protected:

	FNumberFormattingOptions FormatOptions;

	FActiveGameplayEffectHandle CurrentActiveHandle;

	FTimerHandle TimerHandle;



	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> DurationProgressBar;

	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> DurationTextBlock;

	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> StackTextBlock;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UAbilitySystemComponent> ASC;



	UFUNCTION()
	void OnEffectStackChanged(FActiveGameplayEffectHandle EffectHandle, int NewStackCount, int PreviousStackCount);



	UFUNCTION(BlueprintNativeEvent)
	void HandleEffectStackChanged(FActiveGameplayEffectHandle EffectHandle, int NewStackCount, int PreviousStackCount);
	virtual void HandleEffectStackChanged_Implementation(FActiveGameplayEffectHandle EffectHandle, int NewStackCount, int PreviousStackCount);

	UFUNCTION(BlueprintNativeEvent)
	void HandleEffectRemoved(const FGameplayEffectRemovalInfo& EffectRemovalInfo);
	virtual void HandleEffectRemoved_Implementation(const FGameplayEffectRemovalInfo& EffectRemovalInfo);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HandleEffectTimeChanged();
	virtual void HandleEffectTimeChanged_Implementation();



	UFUNCTION(BlueprintCallable)
	void GetEffectDurationAndRemainingTime(float& Duration, float& RemainingTime);

	UFUNCTION()
	void CleanUpASC(const FActiveGameplayEffectHandle& EffectHandle);

	UFUNCTION()
	void CleanUpTimer();

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};

