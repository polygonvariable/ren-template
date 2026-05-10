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
#include "EffectDurationTagWidget.generated.h"

// Forward Declarations
class UTextBlock;
class UProgressBar;



/**
 *
 */
UCLASS(Abstract)
class UEffectDurationTagWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag CaptureDurationTag;



	UFUNCTION(BlueprintCallable)
	void RegisterASC(UAbilitySystemComponent* TargetASC);

protected:

	FGameplayTagContainer DurationTags;

	FNumberFormattingOptions FormatOptions;

	FTimerHandle TimerHandle;



	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> DurationProgressBar;

	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> DurationTextBlock;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UAbilitySystemComponent> ASC;



	UFUNCTION()
	void OnTagCountChanged(const FGameplayTag GameplayTag, int NewCount);



	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HandleEffectTimeChanged();
	virtual void HandleEffectTimeChanged_Implementation();



	UFUNCTION(BlueprintCallable)
	void GetEffectDurationAndRemainingTime(float& Duration, float& RemainingTime);

	UFUNCTION()
	void CleanUpASC();

	UFUNCTION()
	void CleanUpTimer();

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};





/**
 *
 */
UCLASS(Abstract)
class UPlayerEffectDurationTagWidget : public UEffectDurationTagWidget
{

	GENERATED_BODY()

protected:

	UFUNCTION()
	UAbilitySystemComponent* GetASCFromPlayer(AActor* Player);

	UFUNCTION()
	void RegisterPlayer();

	UFUNCTION()
	void CleanUpPlayer();

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};