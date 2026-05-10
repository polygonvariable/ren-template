// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "AttributeSet.h"
#include "GameplayTagContainer.h"

// Generated Headers
#include "CharacterHealthComponent.generated.h"

// Forward Declarations
class UGameplayEffect;
class UAbilitySystemComponent;
struct FGameplayEventData;


/**
 * 
 */
UCLASS(MinimalAPI, Meta = (BlueprintSpawnableComponent))
class UCharacterHealthComponent : public UActorComponent
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

	UPROPERTY(EditAnywhere)
	FGameplayAttribute HealthAttribute;

	UPROPERTY(EditAnywhere)
	FGameplayTag Event_RevivedTag;

	UPROPERTY(EditAnywhere)
	FGameplayTag Event_DiedTag;


	void HandleOnRevived(const FGameplayEventData* EventData);
	void HandleOnDied(const FGameplayEventData* EventData);

	void ApplyGameplayEffect(const TSubclassOf<UGameplayEffect>& EffectClass);
	UAbilitySystemComponent* GetAbilitySystemComponent() const;

};

