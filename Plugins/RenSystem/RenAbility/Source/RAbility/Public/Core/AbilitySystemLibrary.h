// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"

// Generated Headers
#include "AbilitySystemLibrary.generated.h"

// Forward Declarations
class UAbilitySystemComponent;


/**
 * Blueprint helper library for AbilitySystem
 */
UCLASS()
class UAbilitySystemLibrary : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()

public:

	static void ApplyGameplayEffectToTarget(AActor* Target, AActor* Caster, AActor* Causer, TSubclassOf<UGameplayEffect> EffectClass, int Level, TFunctionRef<void(FGameplayEffectContextHandle&)> CustomizeEffectContext = [](FGameplayEffectContextHandle& Context) {});


	UE_DEPRECATED(5.4, "Use Method provided from RAbilitySystemComponent instead")
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "Target", DeprecatedFunction))
	static void CancelAbilityWithTags(AActor* Target, UPARAM(ref) const FGameplayTagContainer& InTags);

	UE_DEPRECATED(5.4, "Use Method provided from RAbilitySystemComponent instead")
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "Target", DeprecatedFunction))
	static void CancelAbilityWithoutTags(AActor* Target, UPARAM(ref) const FGameplayTagContainer& InTags);


	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DefaultToSelf = "Target"))
	static FGameplayTagContainer GetDynamicGameplayTags(AActor* Target, UPARAM(ref) const FGameplayAbilitySpecHandle& Handle);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DefaultToSelf = "Ability"))
	static FGameplayTagContainer GetDynamicGameplayTagsFromAbility(UGameplayAbility* Ability);


	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FGameplayTag GetFirstGameplayTag(const FGameplayTagContainer& Container);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FGameplayTag GetFirstMatchingGameplayTag(const FGameplayTagContainer& Container, FGameplayTag TagToMatch);


	UE_DEPRECATED(5.4, "Use Method provided from RAbilitySystemComponent instead")
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DefaultToSelf = "Target", DeprecatedFunction))
	static bool IsGameplayEffectInhibited(AActor* Target, UPARAM(ref) FActiveGameplayEffectHandle& Handle);

	UE_DEPRECATED(5.4, "Use Method provided from RAbilitySystemComponent instead")
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "InTarget", DeprecatedFunction))
	static void InhibitGameplayEffect(AActor* InTarget, UPARAM(ref) FActiveGameplayEffectHandle& InHandle, bool bInInhibit, bool bInInvokeCue, FActiveGameplayEffectHandle& OutHandle);

	UE_DEPRECATED(5.4, "Use Method provided from RAbilitySystemComponent instead")
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DefaultToSelf = "Target", DeprecatedFunction))
	static bool IsGameplayEffectValid(AActor* Target, UPARAM(ref) FActiveGameplayEffectHandle& Handle);


	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DefaultToSelf = "Target"))
	static int GetGameplayEffectCount(AActor* Target);

protected:

	static UAbilitySystemComponent* GetAbilitySystemComponent(AActor* Target);

};

