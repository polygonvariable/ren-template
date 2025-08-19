// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "InstancedStruct.h"
#include "Abilities/GameplayAbility.h"
#include "AttributeSet.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.h"

// Project Headers
#include "RenCoreLibrary/Public/LogMacro.h"

// Generated Headers
#include "BaseAttributeAbility.generated.h"

// Forward Declarations



/**
 * TODO
 * This will be removed as custom ULevelMagnitudeCalculation class added
 * to override the new max attribute values. which doesnt require
 * long running ability or effect.
 *
 */
UCLASS()
class UBaseAttributeAbility : public UGameplayAbility
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttribute LevelAttribute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> TargetEffectClass;

	UPROPERTY(BlueprintReadOnly)
	FActiveGameplayEffectHandle ActiveEffectHandle;



	virtual void OnLevelAttributeChanged(const FOnAttributeChangeData& Data);

public:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

};


USTRUCT(BlueprintType)
struct FAttributeEffects
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSet<TSubclassOf<UGameplayEffect>> Effects;

};


/**
 *
 */
UCLASS(Blueprintable, ClassGroup = (Custom), Meta = (BlueprintSpawnableComponent))
class UAttributeObserverComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	UAttributeObserverComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttribute ObserveAttribute;


	UFUNCTION(BlueprintCallable)
	void RegisterObserver();

	UFUNCTION(BlueprintCallable)
	void UnregisterObserver();

protected:

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UAbilitySystemComponent> AbilityComponent;


	virtual void OnAttributeChanged(const FOnAttributeChangeData& Data);

public:

	virtual void Activate(bool bReset) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnValueChanged, int, NewValue);
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnValueChanged OnValueChanged;

};


/**
 *
 */
UCLASS(Blueprintable, ClassGroup = (Custom), Meta = (BlueprintSpawnableComponent))
class UAttributeLevelObserversComponent : public UAttributeObserverComponent
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> TargetEffectClass;

	virtual void OnAttributeChanged(const FOnAttributeChangeData& Data) override;

};












