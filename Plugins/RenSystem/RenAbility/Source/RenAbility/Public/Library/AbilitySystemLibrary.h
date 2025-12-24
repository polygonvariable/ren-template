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
 * 
 * 
 */
UCLASS()
class RENABILITY_API UAbilitySystemLibrary : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()

public:

	static void ApplyGameplayEffectToTarget(AActor* Target, AActor* Caster, AActor* Causer, TSubclassOf<UGameplayEffect> EffectClass, int Level = 1, TFunctionRef<void(FGameplayEffectContextHandle&)> CustomizeEffectContext = [](FGameplayEffectContextHandle& Context) {});


	UFUNCTION(BlueprintCallable)
	static void CancelAbilityWithTags(AActor* Target, UPARAM(ref) const FGameplayTagContainer& InTags);

	UFUNCTION(BlueprintCallable)
	static void CancelAbilityWithoutTags(AActor* Target, UPARAM(ref) const FGameplayTagContainer& InTags);


	UFUNCTION(BlueprintCallable)
	static void InhibitGameplayEffect(AActor* InTarget, UPARAM(ref) FActiveGameplayEffectHandle& InHandle, bool bInInhibit, bool bInInvokeCue, FActiveGameplayEffectHandle& OutHandle);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static bool IsGameplayEffectValid(AActor* Target, UPARAM(ref) FActiveGameplayEffectHandle& Handle);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static int GetGameplayEffectCount(AActor* Target);

protected:

	static UAbilitySystemComponent* GetAbilitySystemComponent(AActor* Target);
	

};

