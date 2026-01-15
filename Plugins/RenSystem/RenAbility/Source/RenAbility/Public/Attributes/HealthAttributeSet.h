// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

// Project Headers
#include "AbilityMacro.h"

// Generated Headers
#include "HealthAttributeSet.generated.h"



/**
 * 
 * 
 * 
 */
UCLASS()
class UHealthAttribute : public UAttributeSet
{

	GENERATED_BODY()

public:

	ATTRIBUTE_ACCESSORS(UHealthAttribute, Health);
	ATTRIBUTE_ACCESSORS(UHealthAttribute, MaxHealth);

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;


	// ~ UAttributeSet
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	// ~ End of UAttributeSet

protected:

	// ~ UAttributeSet
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ~ End of UAttributeSet


	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

};



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






/**
 *
 *
 *
 */
UCLASS()
class UMovementSet : public UAttributeSet
{

	GENERATED_BODY()

public:

	ATTRIBUTE_ACCESSORS(UMovementSet, GroundSpeed);
	ATTRIBUTE_ACCESSORS(UMovementSet, MaxGroundSpeed);

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_GroundSpeed)
	FGameplayAttributeData GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxGroundSpeed)
	FGameplayAttributeData MaxGroundSpeed;


	// ~ UAttributeSet
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	// End of UAttributeSet

protected:

	UFUNCTION()
	void OnRep_GroundSpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxGroundSpeed(const FGameplayAttributeData& OldValue);

	// ~ UAttributeSet
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ~ End of UAttributeSet

};

