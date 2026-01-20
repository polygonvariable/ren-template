// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

// Project Headers
#include "AbilityMacro.h"

// Generated Headers
#include "MovementSet.generated.h"



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

	ATTRIBUTE_ACCESSORS(UMovementSet, FlySpeed);
	ATTRIBUTE_ACCESSORS(UMovementSet, MaxFlySpeed);

	ATTRIBUTE_ACCESSORS(UMovementSet, SwimSpeed);
	ATTRIBUTE_ACCESSORS(UMovementSet, MaxSwimSpeed);

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_GroundSpeed)
	FGameplayAttributeData GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxGroundSpeed)
	FGameplayAttributeData MaxGroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_FlySpeed)
	FGameplayAttributeData FlySpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxFlySpeed)
	FGameplayAttributeData MaxFlySpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_SwimSpeed)
	FGameplayAttributeData SwimSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxSwimSpeed)
	FGameplayAttributeData MaxSwimSpeed;


	// ~ UAttributeSet
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	// End of UAttributeSet

protected:

	UFUNCTION()
	void OnRep_GroundSpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxGroundSpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_FlySpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxFlySpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_SwimSpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxSwimSpeed(const FGameplayAttributeData& OldValue);

	// ~ UAttributeSet
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ~ End of UAttributeSet

};

