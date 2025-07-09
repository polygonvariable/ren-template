// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "RenEnvironment/Public/StampedEvent/StampedEventType.h"

// Generated Headers
#include "StampedEventSubsystem.generated.h"

// Forward Declarations
// class UStorage;


/**
 *
 */
UCLASS()
class UStampedEventSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction), Category = "Stamped Event Subsystem|Action")
	bool AddChange(const FName Id, const bool bForceAdd = false);
	virtual bool AddChange_Implementation(const FName Id, const bool bForceAdd = false);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction), Category = "Stamped Event Subsystem|Action")
	bool RemoveChange(const FName Id);
	virtual bool RemoveChange_Implementation(const FName Id);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure, Meta = (ForceAsFunction), Category = "Stamped Event Subsystem|Action")
	bool HasChanged(const FName Id);
	virtual bool HasChanged_Implementation(const FName Id);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction), Category = "Stamped Event Subsystem|Action")
	bool GetEventDate(const FName Id, FDateTime& EventDate);
	virtual bool GetEventDate_Implementation(const FName Id, FDateTime& EventDate);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure, Meta = (ForceAsFunction), Category = "Stamped Event Subsystem|Action")
	EStampedEventStatus GetEventStatus(const FName Id);
	virtual EStampedEventStatus GetEventStatus_Implementation(const FName Id);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure, Meta = (ForceAsFunction), Category = "Stamped Event Subsystem|Action")
	EStampedEventCooldownStatus GetEventCooldownStatus(const FName Id, const bool bIsOnlyOnce, const FTimespan CooldownTime);
	virtual EStampedEventCooldownStatus GetEventCooldownStatus_Implementation(const FName Id, const bool bIsOnlyOnce, const FTimespan CooldownTime);

protected:

	//UPROPERTY(BlueprintReadOnly, Category = "Stamped Event Subsystem|Runtime")
	//UStorage* Storage;

};

