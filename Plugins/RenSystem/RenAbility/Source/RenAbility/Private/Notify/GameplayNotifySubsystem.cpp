// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Notify/GameplayNotifySubsystem.h"

// Engine Headers

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"
#include "RenAbility/Public/Notify/GameplayNotifyNetActor.h"
#include "RenAbility/Public/Notify/GameplayNotifyPacket.h"



bool UGameplayNotifySubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UGameplayNotifySubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	NetworkActor = InWorld.SpawnActor<AGameplayNotifyNetActor>();
}

void UGameplayNotifySubsystem::Deinitialize()
{
	if (IsValid(NetworkActor))
	{
		NetworkActor->Destroy();
	}
	NetworkActor = nullptr;

	Super::Deinitialize();
}

FGameplayNotifyDelegate& UGameplayNotifySubsystem::GetNotifyDelegate(FGameplayTag NotifyTag)
{
	FGameplayNotifyDelegate& Delegate = RegisteredNotifies.FindOrAdd(NotifyTag);
	return Delegate;
}

FGameplayNotifyDelegate* UGameplayNotifySubsystem::FindNotifyDelegate(FGameplayTag NotifyTag)
{
	return RegisteredNotifies.Find(NotifyTag);
}

void UGameplayNotifySubsystem::SendNotify(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Data, bool bReliable)
{
	if (!IsValid(NetworkActor) || !NetworkActor->HasAuthority())
	{
		return;
	}

	NetworkActor->SendNotify(NotifyTag, Data, bReliable);
}

void UGameplayNotifySubsystem::BroadcastNotify(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Data)
{
	const FGameplayNotifyDelegate* Delegate = RegisteredNotifies.Find(NotifyTag);
	if (Delegate)
	{
		Delegate->Broadcast(Data);
	}
}


