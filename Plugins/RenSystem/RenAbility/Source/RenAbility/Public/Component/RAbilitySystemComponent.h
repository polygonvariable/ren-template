// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"

// Project Headers

// Generated Headers
#include "RAbilitySystemComponent.generated.h"

// Forward Declarations



/**
 * 
 * 
 * 
 */
UCLASS(MinimalAPI)
class URAbilitySystemComponent : public UAbilitySystemComponent
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Cancel Abilities By Tags"))
	void BP_CancelAbilitiesByTags(const FGameplayTagContainer& Tags);

	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Cancel Abilities Without Tags"))
	void BP_CancelAbilitiesWithoutTags(const FGameplayTagContainer& Tags);

	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Cancel Abilities With Dynamic Tags"))
	void BP_CancelAbilitiesWithDynamicTags(const FGameplayTagContainer& Tags);

	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Cancel Abilities With Handle"))
	void BP_CancelAbilityWithHandle(const FGameplayAbilitySpecHandle& Handle);

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

