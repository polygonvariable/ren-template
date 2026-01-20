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

// Declare Gameplay Tags
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Health_Damage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Health_Heal);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Health_Immunity);

// Forward Declarations
class UGameplayNotifySubsystem;



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

	DECLARE_MULTICAST_DELEGATE(FOnDamaged)
	FOnDamaged OnDamage;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MinHealth)
	FGameplayAttributeData MinHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Heal;


	// ~ UAttributeSet
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	// ~ End of UAttributeSet

protected:

	bool bIsDead = false;

	UPROPERTY()
	TWeakObjectPtr<UGameplayNotifySubsystem> GameplayNotifySubsystem;

	void UpdateHealth(float Value, float Multiplier);

	bool IsDead() const;

	void UpdateDeathState();

	UGameplayNotifySubsystem* GetGameplayNotifySubsystem();

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

