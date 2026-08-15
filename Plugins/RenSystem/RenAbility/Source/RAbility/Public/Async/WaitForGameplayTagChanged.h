// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"

// Generated Headers
#include "WaitForGameplayTagChanged.generated.h"

// Forward Declarations
class UAbilitySystemComponent;


/**
 *
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class UWaitForGameplayTagChanged : public UBlueprintAsyncActionBase
{

	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTagChanged, int, Count);
	UPROPERTY(BlueprintAssignable)
	FOnTagChanged OnTagChanged;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", DefaultToSelf = "Actor"))
	static UWaitForGameplayTagChanged* WaitForGameplayTagChanged(AActor* Actor, FGameplayTag Tag);

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:

	TWeakObjectPtr<AActor> TargetActor;
	TWeakObjectPtr<UAbilitySystemComponent> TargetASC;
	FGameplayTag TargetTag;

	UFUNCTION()
	void HandleTagChanged(const FGameplayTag Tag, int Count);

protected:

	virtual void Activate() override;

};

