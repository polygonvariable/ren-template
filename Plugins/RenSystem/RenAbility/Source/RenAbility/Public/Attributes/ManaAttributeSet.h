// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

// Project Headers
#include "AbilityMacro.h"

// Generated Headers
#include "ManaAttributeSet.generated.h"



/**
 * 
 */
UCLASS()
class UManaAttributeSet : public UAttributeSet
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Mana;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData ManaMax;


	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

public:

	ATTRIBUTE_ACCESSORS(UManaAttributeSet, Mana)
	ATTRIBUTE_ACCESSORS(UManaAttributeSet, ManaMax)

};

