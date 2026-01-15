// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AttributeSet.h"
#include "GameplayEffectTypes.h"

// Project Headers

// Generated Headers
#include "WaitForAttributePerChange.generated.h"

// Forward Declarations
class UAbilitySystemComponent;



/**
 * 
 * 
 */
UENUM(BlueprintType)
enum class EChangeType : uint8
{
	Equal UMETA(DisplayName = "Equal"),
	NotEqual UMETA(DisplayName = "Not Equal"),
	LessThan UMETA(DisplayName = "Less Than"),
	GreaterThan UMETA(DisplayName = "Greater Than"),
};



/**
 * 
 * 
 */
UCLASS(BlueprintType, Meta = (ExposedAsyncProxy = AsyncTask))
class UWaitForAttributePerChange : public UBlueprintAsyncActionBase
{

	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnValueChanged, float, NewValue, float, OldValue);
	UPROPERTY(BlueprintAssignable)
	FOnValueChanged OnChanged;

	UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = "true", DefaultToSelf = "InActor"))
	static UWaitForAttributePerChange* WaitForAttributePerChange(AActor* InActor, FGameplayAttribute InAttribute, EChangeType InChangeType, bool bInNegate = false);

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:

	TWeakObjectPtr<AActor> Actor;
	TWeakObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayAttribute Attribute;
	EChangeType ChangeType;
	bool bNegate = false;

	float OldValue = 0.0f;

	void HandleValueChanged(const FOnAttributeChangeData& Data);
	bool CheckCondition(float NewValue);

protected:

	virtual void Activate() override;

};

