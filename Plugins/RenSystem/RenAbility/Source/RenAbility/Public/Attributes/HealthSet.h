// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"

// Project Headers
#include "AbilityMacro.h"

// Generated Headers
#include "HealthSet.generated.h"

// Forward Declarations
// class UGameplayNotifySubsystem;

// Declare Gameplay Tags
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Health_Damaged);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Health_Healed);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Health_Immunity);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Health_Fallen);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Health_Restored);



/**
 * 
 * 
 * 
 */
UCLASS()
class UHealthSet : public UAttributeSet
{

	GENERATED_BODY()

public:

	ATTRIBUTE_ACCESSORS(UHealthSet, Health);
	ATTRIBUTE_ACCESSORS(UHealthSet, MinHealth);
	ATTRIBUTE_ACCESSORS(UHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UHealthSet, Damage);
	ATTRIBUTE_ACCESSORS(UHealthSet, Heal);

public:

	UPROPERTY(ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;

	UPROPERTY(ReplicatedUsing = OnRep_MinHealth)
	FGameplayAttributeData MinHealth;

	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;

	UPROPERTY()
	FGameplayAttributeData Damage;

	UPROPERTY()
	FGameplayAttributeData Heal;


	// ~ UAttributeSet
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	// ~ End of UAttributeSet

protected:

	bool bIsDead = true;

	void UpdateHealthState();
	void UpdateHealth(float Value, float Multiplier);
	void BroadcastHealthState();

	void HandleDamage(const FGameplayEffectModCallbackData& Data);
	void HandleHeal(const FGameplayEffectModCallbackData& Data);
	void BroadcastHealthChanged(const FGameplayEffectModCallbackData& Data, float Magnitude, const FGameplayTag& EventTag);


	//UPROPERTY()
	//TWeakObjectPtr<UGameplayNotifySubsystem> GameplayNotifySubsystem;
	//UGameplayNotifySubsystem* GetGameplayNotifySubsystem();

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MinHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	// ~ UAttributeSet
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ~ End of UAttributeSet

};

