// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

// Project Headers
#include "AbilityMacro.h"

// Generated Headers
#include "StaminaSet.generated.h"



/**
 * 
 * 
 * 
 */
UCLASS()
class UStaminaSet : public UAttributeSet
{

	GENERATED_BODY()

public:

	ATTRIBUTE_ACCESSORS(UStaminaSet, Stamina);
	ATTRIBUTE_ACCESSORS(UStaminaSet, MaxStamina);

public:

	//DECLARE_MULTICAST_DELEGATE(FOnStaminaExhausted);
	//FOnStaminaExhausted OnStaminaExhausted;

	//DECLARE_MULTICAST_DELEGATE(FOnStaminaRefilled);
	//FOnStaminaRefilled OnStaminaRefilled;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;


	// ~ UAttributeSet
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	// End of UAttributeSet

protected:

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldValue);

	// ~ UAttributeSet
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ~ End of UAttributeSet

};

