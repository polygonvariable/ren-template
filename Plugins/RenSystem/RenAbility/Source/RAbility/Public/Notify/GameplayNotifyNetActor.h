// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Generated Headers
#include "GameplayNotifyNetActor.generated.h"

// Forward Declarations
class UGameplayNotifySubsystem;

struct FGameplayNotifyPacket;



/*
 * Actor to manage event replication send by the UGameplayNotifySubsystem.
 * 
 * 
 */
UCLASS()
class AGameplayNotifyNetActor : public AInfo
{

	GENERATED_BODY()

public:

	AGameplayNotifyNetActor();

	void SendNotify(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Data, bool bReliable);

	// ~ AInfo
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~ End of AInfo

protected:

	UPROPERTY()
	TWeakObjectPtr<UGameplayNotifySubsystem> NotifySubsystem;


	UFUNCTION(NetMulticast, Reliable)
	void SendNotifyReliable(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Data);
	virtual void SendNotifyReliable_Implementation(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Data);

	UFUNCTION(NetMulticast, Unreliable)
	void SendNotifyUnreliable(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Data);
	virtual void SendNotifyUnreliable_Implementation(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Data);

	void SendNotifyInternal(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Data);

	// ~ AInfo
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	// ~ End of AInfo

};

