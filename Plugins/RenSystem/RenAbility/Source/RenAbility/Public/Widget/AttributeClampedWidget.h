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
#include "AttributeClampedWidget.generated.h"

// Forward Declarations
class UTextBlock;
class UProgressBar;
class UPanelWidget;
class URAbilitySystemComponent;



/**
 *
 */
UCLASS(Abstract)
class UAttributeClampedWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttribute BaseAttribute;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttribute MaxAttribute;



	UFUNCTION()
	void RegisterASC(URAbilitySystemComponent* TargetASC);

protected:

	UPROPERTY(BlueprintReadOnly)
	float CurrentValue = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	float CurrentMax = 0.0f;



	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TitleTextBlock;

	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ValueTextBlock;

	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> ValueProgressBar;



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
class UPlayerAttributeClampedWidget : public UAttributeClampedWidget
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

