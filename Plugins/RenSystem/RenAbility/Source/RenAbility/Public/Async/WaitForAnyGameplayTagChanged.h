// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"

// Project Headers

// Generated Headers
#include "WaitForAnyGameplayTagChanged.generated.h"

// Forward Declarations
class UAbilitySystemComponent;



/**
 *
 */
UCLASS(BlueprintType, Meta = (ExposedAsyncProxy = AsyncTask))
class UWaitForAnyGameplayTagChanged : public UBlueprintAsyncActionBase
{

	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTagChanged, FGameplayTag, Tag, int, Count);
	UPROPERTY(BlueprintAssignable)
	FOnTagChanged OnTagChanged;

	UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = "true", DefaultToSelf = "Actor"))
	static UWaitForAnyGameplayTagChanged* WaitForAnyGameplayTagChanged(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:

	TWeakObjectPtr<AActor> TargetActor;
	TWeakObjectPtr<UAbilitySystemComponent> TargetASC;

	UFUNCTION()
	void HandleTagChanged(const FGameplayTag Tag, int Count);

protected:

	virtual void Activate() override;

};

