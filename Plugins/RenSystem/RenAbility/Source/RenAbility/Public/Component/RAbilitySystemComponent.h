// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"

// Project Headers

// Generated Headers
#include "RAbilitySystemComponent.generated.h"

// Forward Declarations


/**
 * 
 */
UCLASS()
class RENABILITY_API URAbilitySystemComponent : public UAbilitySystemComponent
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	FGameplayAbilitySpecHandle GiveAbilityWithDynamicTags(TSubclassOf<UGameplayAbility> AbilityClass, int Level = 1, int InputID = -1, const FGameplayTagContainer& DynamicTags = FGameplayTagContainer());

	UFUNCTION()
	void AddAggregatedActor(const FGameplayAttribute& Attribute, AActor* Actor);

	UFUNCTION()
	void RemoveAggregatedActor(const FGameplayAttribute& Attribute, AActor* Actor);

	UFUNCTION()
	float GetAggregatedNumericAttribute(const FGameplayAttribute& Attribute);



	UFUNCTION(BlueprintCallable)
	void BP_AddReplicatedGameplayTags(const FGameplayTagContainer& Tags);

	UFUNCTION(BlueprintCallable)
	void BP_RemoveReplicatedGameplayTags(const FGameplayTagContainer& Tags);



	UFUNCTION(BlueprintCallable)
	void BP_CancelAbilitiesByTags(const FGameplayTagContainer& WithTags);


	UFUNCTION()
	UAnimInstance* GetActorAnimInstance();

	UFUNCTION(BlueprintCallable)
	float AnimPlayMontage(UAnimMontage* Montage, float PlayRate = 1.0f, float StartTime = 0.0f, bool bStopAllMontages = false);

	UFUNCTION(BlueprintCallable)
	void AnimStopMontage(UAnimMontage* Montage, float BlendOutTime = 0.0f);

protected:

	TSet<TObjectPtr<UAnimMontage>> ActiveMontages;

	TMap<FGameplayAttribute, TSet<TWeakObjectPtr<AActor>>> AggregatedActors;

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAggregatedActorAdded, AActor*, Actor);
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAggregatedActorAdded OnAggregatedActorAdded;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAggregatedActorRemoved, AActor*, Actor);
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAggregatedActorRemoved OnAggregatedActorRemoved;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAggregatedRefresh);
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAggregatedRefresh OnAggregatedRefresh;

};

