// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "ActiveGameplayEffectHandle.h"

// Generated Headers
#include "HealthComponent.generated.h"

// Forward Declarations
class UAbilitySystemComponent;
class UHealthSet;


/**
 * 
 */
UCLASS(MinimalAPI, Meta = (BlueprintSpawnableComponent))
class UHealthComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	void InitializeManager();
	void DeinitializeManager();

	// ~ UActorComponent
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~ End of UActorComponent

protected:

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystem;

	UPROPERTY()
	TObjectPtr<const UHealthSet> HealthSet;

	FActiveGameplayEffectHandle DeadHandle;


	void HandleOnRevived();
	void HandleOnDied();

};

