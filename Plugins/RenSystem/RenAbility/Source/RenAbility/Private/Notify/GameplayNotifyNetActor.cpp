// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Notify/GameplayNotifyNetActor.h"

// Engine Headers

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"
#include "RenAbility/Public/Notify/GameplayNotifySubsystem.h"



AGameplayNotifyNetActor::AGameplayNotifyNetActor()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	SetReplicatingMovement(false);
}

void AGameplayNotifyNetActor::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		NotifySubsystem = World->GetSubsystem<UGameplayNotifySubsystem>();
	}
}

void AGameplayNotifyNetActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	NotifySubsystem.Reset();

	Super::EndPlay(EndPlayReason);
}

void AGameplayNotifyNetActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AGameplayNotifyNetActor::SendNotifyInternal(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Notify)
{
	UGameplayNotifySubsystem* Subsystem = NotifySubsystem.Get();
	if (IsValid(Subsystem))
	{
		Subsystem->BroadcastNotify(NotifyTag, Notify);
	}
}

void AGameplayNotifyNetActor::SendNotify(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Notify, bool bReliable)
{
	if (bReliable)
	{
		SendNotifyReliable(NotifyTag, Notify);
	}
	else
	{
		SendNotifyUnreliable(NotifyTag, Notify);
	}
}

void AGameplayNotifyNetActor::SendNotifyReliable_Implementation(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Notify)
{
	SendNotifyInternal(NotifyTag, Notify);
}

void AGameplayNotifyNetActor::SendNotifyUnreliable_Implementation(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Notify)
{
	SendNotifyInternal(NotifyTag, Notify);
}

