// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"

// Project Headers
#include "AbilityMacro.h"

// Generated Headers
#include "DefenseSet.generated.h"


/**
 *
 */
UCLASS()
class UDefenseSet : public UAttributeSet
{

	GENERATED_BODY()

public:

	ATTRIBUTE_ACCESSORS(UDefenseSet, Physical)
	ATTRIBUTE_ACCESSORS(UDefenseSet, Elemental)

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

