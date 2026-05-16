// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "AbilitySystemComponent.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

// Generated Headers
#include "RAbilitySystemComponent.generated.h"


/**
 * 
 */
UCLASS(MinimalAPI)
class URAbilitySystemComponent : public UAbilitySystemComponent
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Try Give Ability With Source"))
	FGameplayAbilitySpecHandle BP_TryGiveAbilityWithSource(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level, int32 InputID, const FGameplayTagContainer& Tags, UObject* SourceObject);

	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Cancel Abilities With Tags"))
	void BP_CancelAbilitiesWithTags(const FGameplayTagContainer& Tags);

	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Cancel Abilities Without Tags"))
	void BP_CancelAbilitiesWithoutTags(const FGameplayTagContainer& Tags);

	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Cancel Abilities With Dynamic Tags"))
	void BP_CancelAbilitiesWithDynamicTags(const FGameplayTagContainer& Tags);

	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Cancel Abilities With Handle"))
	void BP_CancelAbilityWithHandle(const FGameplayAbilitySpecHandle& Handle);



	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (DisplayName = "Is Gameplay Effect Active"))
	bool BP_IsGameplayEffectActive(UPARAM(ref) FActiveGameplayEffectHandle& Handle) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (DisplayName = "Is Gameplay Effect Inhibited"))
	bool BP_IsGameplayEffectInhibited(UPARAM(ref) FActiveGameplayEffectHandle& Handle) const;

	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Inhibit Gameplay Effect"))
	void BP_InhibitGameplayEffect(UPARAM(ref) FActiveGameplayEffectHandle& InHandle, bool bInInhibit, bool bInInvokeCue, FActiveGameplayEffectHandle& OutHandle);



	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Get Owned Gameplay Tags"))
	const FGameplayTagContainer& BP_GetOwnedGameplayTags() const { return GetOwnedGameplayTags(); }


	// ~ UAbilitySystemComponent
	virtual int32 HandleGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload) override;
	// ~ End of UAbilitySystemComponent

protected:

	UPROPERTY(EditAnywhere)
	bool bUseCueBinding = false;

	UPROPERTY(EditAnywhere)
	TObjectPtr<const UGameplayCueBinding> CueBindingAsset = nullptr;


	void TriggerGameplayCue(FGameplayTag EventTag, const FGameplayEventData* Payload);

};


/**
 * 
 */
UCLASS()
class UGameplayCueBinding : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, FGameplayCueTag> TagRedirects;


	const FGameplayCueTag* GetGameplayCueTag(FGameplayTag Tag) const;

};

