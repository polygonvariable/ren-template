// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "ActiveGameplayEffectHandle.h"

// Generated Headers
#include "HealthManagerComponent.generated.h"

// Forward Declarations
class UAbilitySystemComponent;
class UHealthSet;


/**
 * 
 */
UCLASS(MinimalAPI, meta = (BlueprintSpawnableComponent))
class UHealthManagerComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	UHealthManagerComponent();

	// ~ UActorComponent
	virtual void Activate(bool bReset) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~ End of UActorComponent

protected:

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystem;

	UPROPERTY()
	TObjectPtr<const UHealthSet> HealthSet;

	FActiveGameplayEffectHandle DeadHandle;


	// ~ Binding
	void HandleOnRevived();
	void HandleOnDied();
	// ~ End of Binding

};

