// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "Subsystems/GameInstanceSubsystem.h"

// Generated Headers
#include "BroadcastSubsystem.generated.h"

// Delegate Declaration
DECLARE_MULTICAST_DELEGATE_OneParam(FBroadcastDelegate, const FInstancedStruct& /* Payload */);


/**
 * 
 */
UCLASS(MinimalAPI)
class UBroadcastSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	CBROADCAST_API void RegisterBroadcast(FGameplayTag Tag, FBroadcastDelegate::FDelegate&& Callback);
	CBROADCAST_API void UnregisterBroadcast(const FGameplayTag& Tag, UObject* Target);

	UFUNCTION(BlueprintCallable)
	CBROADCAST_API void BroadcastMessage(const FGameplayTag& Tag, const FInstancedStruct& Payload);


	template<typename T>
	void BroadcastMessage(const FGameplayTag& Tag, const T& Payload)
	{
		BroadcastMessage(Tag, FInstancedStruct::Make(Payload));
	}

protected:

	TMap<FGameplayTag, TPair<FBroadcastDelegate, int>> BroadcastMap;

	TArray<TPair<FGameplayTag, FDelegateHandle>> PendingRemovals;
	bool bIsBroadcasting = false;

public:

	static CBROADCAST_API UBroadcastSubsystem* Get(UWorld* World);
	static CBROADCAST_API UBroadcastSubsystem* Get(UGameInstance* GameInstance);

};

