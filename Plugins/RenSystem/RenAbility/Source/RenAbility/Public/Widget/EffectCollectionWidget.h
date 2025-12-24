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
#include "EffectCollectionWidget.generated.h"

// Forward Declarations
class UTextBlock;
class UPanelWidget;
class UEffectCollectionEntryWidget;


/**
 *
 */
UCLASS(Abstract)
class UEffectCollectionWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag CaptureEffectTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UEffectCollectionEntryWidget> EntryWidgetClass;



	UFUNCTION(BlueprintCallable)
	void RegisterASC(UAbilitySystemComponent* TargetASC);

protected:

	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UPanelWidget> EffectStackPanel;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UAbilitySystemComponent> ASC;



	UFUNCTION()
	void OnEffectAdded(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);

	UFUNCTION()
	void OnEffectRemoved(const FActiveGameplayEffect& EffectRemoved);



	UFUNCTION(BlueprintNativeEvent)
	void HandleEffectAdded(const FGameplayEffectSpec& SpecApplied, const FActiveGameplayEffectHandle& ActiveHandle);
	virtual void HandleEffectAdded_Implementation(const FGameplayEffectSpec& SpecApplied, const FActiveGameplayEffectHandle& ActiveHandle);

	UFUNCTION(BlueprintNativeEvent)
	void HandleEffectRemoved(const FActiveGameplayEffect& EffectRemoved);
	virtual void HandleEffectRemoved_Implementation(const FActiveGameplayEffect& EffectRemoved);



	UFUNCTION()
	void CleanUpASC();

protected:

	virtual void NativeDestruct() override;

};





/**
 *
 */
UCLASS(Abstract)
class UPlayerEffectCollectionWidget : public UEffectCollectionWidget
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

