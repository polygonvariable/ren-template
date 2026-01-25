// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"

// Project Headers
#include "RenAbility/Public/Notify/GameplayNotifyPacket.h"

// Generated Headers
#include "WaitForGameplayNotify.generated.h"

// Forward Declarations
struct FGameplayEventData;



/**
 * 
 * 
 * 
 */
UCLASS(BlueprintType, Meta = (ExposedAsyncProxy = AsyncTask))
class UWaitForGameplayNotify : public UBlueprintAsyncActionBase
{

	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNotifyReceived, FGameplayNotifyPacket, Data);
	UPROPERTY(BlueprintAssignable)
	FOnNotifyReceived OnReceived;


	UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContext"))
	static UWaitForGameplayNotify* WaitForGameplayNotify(FGameplayTag InNotifyTag, UObject* InWorldContext);

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:

	FGameplayTag NotifyTag;

	UPROPERTY()
	TWeakObjectPtr<UObject> WorldContext;


	UFUNCTION()
	void HandleNotifyReceived(const FGameplayNotifyPacket& Data);

	// ~ UBlueprintAsyncActionBase
	virtual void Activate() override;
	// ~ End of UBlueprintAsyncActionBase

};

