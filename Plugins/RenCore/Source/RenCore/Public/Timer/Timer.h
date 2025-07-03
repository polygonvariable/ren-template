// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Generated Headers
#include "Timer.generated.h"


/**
 *
 */
UCLASS(Blueprintable)
class RENCORE_API UTimer : public UObject
{

	GENERATED_BODY()

public:
	
	UFUNCTION()
	void StartTimer(const float InTickInterval = 1.0f, const int InTickLimit = 10, const bool bPreserveTime = false);
	
	UFUNCTION()
	void StopTimer(bool bInvalidate = false);

	UFUNCTION()
	void PauseTimer();

	UFUNCTION()
	void UnPauseTimer();
	
	UFUNCTION()
	void RestartTimer(const float InTickInterval = 1.0f, const int InTickLimit = 10);



	UFUNCTION()
	void SetTickInterval(const float InTickInterval = 1.0f);

	UFUNCTION()
	const bool IsActive();

	UFUNCTION()
	const float GetNormalizedAlpha();

protected:

	UPROPERTY()
	FTimerHandle TimerHandle;



	UPROPERTY()
	int TickLimit = 0;

	UPROPERTY()
	float TickInterval = 0.0f;

	UPROPERTY()
	int TickCount = 0;

	UPROPERTY()
	float ElapsedTime = 0.0f;



	UFUNCTION()
	virtual void HandleTick();

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStarted);
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnStarted OnStarted;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTick, float, ElapsedTime);
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnTick OnTick;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCompleted);
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnCompleted OnCompleted;

};

