// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"

// Generated Headers
#include "GameplayNotifySubsystem.generated.h"

// Forward Declaration
class AGameplayNotifyNetActor;

struct FGameplayNotifyPacket;

// Delegate Declaration
DECLARE_MULTICAST_DELEGATE_OneParam(FGameplayNotifyDelegate, const FGameplayNotifyPacket& /* Data */);




/*
 * 
 * 
 * 
 */
UCLASS()
class UGameplayNotifySubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	void SendNotify(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Data, bool bReliable);
	void BroadcastNotify(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Data);

	FGameplayNotifyDelegate& GetNotifyDelegate(FGameplayTag NotifyTag);
	FGameplayNotifyDelegate* FindNotifyDelegate(FGameplayTag NotifyTag);

	// ~ UWorldSubsystem
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Deinitialize() override;
	// ~ End of UWorldSubsystem

protected:

	TMap<FGameplayTag, FGameplayNotifyDelegate> RegisteredNotifies;

	UPROPERTY()
	TObjectPtr<AGameplayNotifyNetActor> NetworkActor;

};

