// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "AttributeSet.h"
#include "Blueprint/UserWidget.h"

// Generated Headers
#include "AttributeClampedUI.generated.h"

// Forward Declarations
class UTextBlock;
class UProgressBar;
struct FOnAttributeChangeData;


/**
 *
 */
UCLASS(Abstract)
class UAttributeClampedUI : public UUserWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FText Title;

	UPROPERTY(EditAnywhere)
	FGameplayAttribute BaseAttribute;

	UPROPERTY(EditAnywhere)
	FGameplayAttribute MaxAttribute;


	UFUNCTION(BlueprintCallable)
	void RegisterAbilitySystem(UAbilitySystemComponent* AbilitySystem);

protected:

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ValueText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ValueProgress;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UAbilitySystemComponent> ASC;


	void HandleValueChanged(const FOnAttributeChangeData& Data);
	void UpdateValue(int Current, int Max);

	void UnregisterAbilitySystem();

protected:

	// ~ UUserWidget
	virtual void NativePreConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

