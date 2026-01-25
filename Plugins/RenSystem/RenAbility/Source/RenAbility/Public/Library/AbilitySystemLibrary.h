// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"

// Generated Headers
#include "AbilitySystemLibrary.generated.h"

// Forward Declarations
class UAbilitySystemComponent;



/**
 *
 * Blueprint helper library for AbilitySystem
 * 
 */
UCLASS()
class RENABILITY_API UAbilitySystemLibrary : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()

public:

	static void ApplyGameplayEffectToTarget(AActor* Target, AActor* Caster, AActor* Causer, TSubclassOf<UGameplayEffect> EffectClass, int Level, TFunctionRef<void(FGameplayEffectContextHandle&)> CustomizeEffectContext = [](FGameplayEffectContextHandle& Context) {});


	UFUNCTION(BlueprintCallable, Meta = (DefaultToSelf = "Target"))
	static void CancelAbilityWithTags(AActor* Target, UPARAM(ref) const FGameplayTagContainer& InTags);

	UFUNCTION(BlueprintCallable, Meta = (DefaultToSelf = "Target"))
	static void CancelAbilityWithoutTags(AActor* Target, UPARAM(ref) const FGameplayTagContainer& InTags);


	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (DefaultToSelf = "Target"))
	static FGameplayTagContainer GetDynamicGameplayTags(AActor* Target, UPARAM(ref) const FGameplayAbilitySpecHandle& Handle);

	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (DefaultToSelf = "Ability"))
	static FGameplayTagContainer GetDynamicGameplayTagsFromAbility(UGameplayAbility* Ability);


	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FGameplayTag GetFirstGameplayTag(const FGameplayTagContainer& Container);



	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (DefaultToSelf = "Target"))
	static bool IsGameplayEffectInhibited(AActor* Target, UPARAM(ref) FActiveGameplayEffectHandle& Handle);

	UFUNCTION(BlueprintCallable, Meta = (DefaultToSelf = "InTarget"))
	static void InhibitGameplayEffect(AActor* InTarget, UPARAM(ref) FActiveGameplayEffectHandle& InHandle, bool bInInhibit, bool bInInvokeCue, FActiveGameplayEffectHandle& OutHandle);

	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (DefaultToSelf = "Target"))
	static bool IsGameplayEffectValid(AActor* Target, UPARAM(ref) FActiveGameplayEffectHandle& Handle);

	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (DefaultToSelf = "Target"))
	static int GetGameplayEffectCount(AActor* Target);

protected:

	static UAbilitySystemComponent* GetAbilitySystemComponent(AActor* Target);
	

};

