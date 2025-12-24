// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"

// Project Headers

// Generated Headers
#include "AnimPlayMontage.generated.h"

// Forward Declarations
class URAbilitySystemComponent;



/**
 *
 */
UCLASS(BlueprintType, Meta = (ExposedAsyncProxy = AsyncTask))
class UAnimPlayMontage : public UBlueprintAsyncActionBase
{

	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStarted);
	UPROPERTY(BlueprintAssignable)
	FOnStarted OnStarted;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCancelled);
	UPROPERTY(BlueprintAssignable)
	FOnCancelled OnCancelled;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompleted, bool, bInterrupted);
	UPROPERTY(BlueprintAssignable)
	FOnCompleted OnCompleted;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBlendedIn);
	UPROPERTY(BlueprintAssignable)
	FOnBlendedIn OnBlendedIn;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlendedOut, bool, bInterrupted);
	UPROPERTY(BlueprintAssignable)
	FOnBlendedOut OnBlendedOut;


	UFUNCTION()
	void OnMontageBlendedIn(UAnimMontage* InMontage);

	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* InMontage, bool bInterrupted);

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* InMontage, bool bInterrupted);



	UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = "true"))
	static UAnimPlayMontage* AnimPlayMontage(URAbilitySystemComponent* InRASC, UAnimMontage* InMontage, float InPlayRate = 1.0f, float InStartTime = 0.0f, float InBlendOutTime = 0.0f, bool bInStopAllMontages = false);



	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:

	FOnMontageBlendedInEnded BlendedInDelegate;

	FOnMontageBlendingOutStarted BlendingOutDelegate;

	FOnMontageEnded MontageEndedDelegate;


	TWeakObjectPtr<URAbilitySystemComponent> RASC;

	TObjectPtr<UAnimMontage> Montage;

	float PlayRate = 1.0f;

	float StartTime = 0.0f;

	float BlendOutTime = 0.0f;

	bool bStopAllMontages = false;

protected:

	virtual void Activate() override;

};

