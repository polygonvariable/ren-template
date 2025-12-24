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
#include "AttributeScalarWidget.generated.h"

// Forward Declarations
class UTextBlock;
class UProgressBar;
class UPanelWidget;
class URAbilitySystemComponent;



/**
 *
 */
UCLASS(Abstract)
class UAttributeScalarWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttribute BaseAttribute;



	UFUNCTION()
	void RegisterASC(URAbilitySystemComponent* TargetASC);

protected:

	UPROPERTY(BlueprintReadOnly)
	float CurrentValue = 0.0f;



	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TitleTextBlock;

	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ValueTextBlock;



	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<URAbilitySystemComponent> ASC;



	UFUNCTION()
	void OnAggregatedRefresh();

	UFUNCTION(BlueprintNativeEvent)
	void HandleValueChanged();
	virtual void HandleValueChanged_Implementation();

	UFUNCTION()
	void CleanUpASC();

protected:

	virtual void NativePreConstruct() override;
	virtual void NativeDestruct() override;

};





/**
 *
 */
UCLASS(Abstract)
class UPlayerAttributeScalarWidget : public UAttributeScalarWidget
{

	GENERATED_BODY()

protected:
	
	UFUNCTION()
	URAbilitySystemComponent* GetASCFromPlayer(AActor* Player);

	UFUNCTION()
	void RegisterPlayer();

	UFUNCTION()
	void CleanUpPlayer();

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};



/**
 * 
 * 
 * 
 */
UCLASS(Abstract)
class UAbilityDebugWidget : public UUserWidget
{

	GENERATED_BODY()

protected:

	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> EffectCount;



	UFUNCTION()
	void CleanUpASC();

	UFUNCTION()
	void RegisterASC(URAbilitySystemComponent* TargetASC);

	UFUNCTION()
	URAbilitySystemComponent* GetASCFromPlayer(AActor* Player);

	UFUNCTION()
	void RegisterPlayer();

	UFUNCTION()
	void CleanUpPlayer();




	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<URAbilitySystemComponent> ASC;


	UFUNCTION()
	void OnEffectApplied(UAbilitySystemComponent* InASC, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle);

	UFUNCTION()
	void OnEffectRemoved(const FActiveGameplayEffect& Effect);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};

