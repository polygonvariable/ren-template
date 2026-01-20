// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

// Project Headers
#include "AbilityMacro.h"

// Generated Headers
#include "DamageSet.generated.h"



/**
 * 
 * 
 * 
 */
UCLASS()
class RENABILITY_API UDamageSet : public UAttributeSet
{

	GENERATED_BODY()

public:

	ATTRIBUTE_ACCESSORS(UDamageSet, Physical)
	ATTRIBUTE_ACCESSORS(UDamageSet, Elemental)

public:

	UPROPERTY(ReplicatedUsing = OnRep_Physical)
	FGameplayAttributeData Physical;

	UPROPERTY(ReplicatedUsing = OnRep_Elemental)
	FGameplayAttributeData Elemental;


	// ~ UAttributeSet
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	// ~ End of UAttributeSet

protected:

	// ~ UAttributeSet
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ~ End of UAttributeSet

	UFUNCTION()
	void OnRep_Physical(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Elemental(const FGameplayAttributeData& OldValue);

};

