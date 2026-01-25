// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Notify/WaitForGameplayNotify.h"

// Engine Headers

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"
#include "RenAbility/Public/Notify/GameplayNotifySubsystem.h"



UWaitForGameplayNotify* UWaitForGameplayNotify::WaitForGameplayNotify(FGameplayTag InNotifyTag, UObject* InWorldContext)
{
	UWaitForGameplayNotify* NewTask = NewObject<UWaitForGameplayNotify>();
	NewTask->NotifyTag = InNotifyTag;
	NewTask->WorldContext = InWorldContext;
	return NewTask;
}

void UWaitForGameplayNotify::HandleNotifyReceived(const FGameplayNotifyPacket& Notify)
{
	OnReceived.Broadcast(Notify);
}

void UWaitForGameplayNotify::Activate()
{
	UObject* Context = WorldContext.Get();
	if (!IsValid(Context))
	{
		return;
	}

	UWorld* World = Context->GetWorld();
	UGameplayNotifySubsystem* Subsystem = World->GetSubsystem<UGameplayNotifySubsystem>();
	if (!IsValid(Subsystem) || !NotifyTag.IsValid())
	{
		return;
	}

	FGameplayNotifyDelegate& Delegate = Subsystem->GetNotifyDelegate(NotifyTag);
	Delegate.AddUObject(this, &UWaitForGameplayNotify::HandleNotifyReceived);
}

void UWaitForGameplayNotify::EndTask()
{
	UObject* Context = WorldContext.Get();
	if (!IsValid(Context))
	{
		return;
	}

	UWorld* World = Context->GetWorld();
	UGameplayNotifySubsystem* Subsystem = World->GetSubsystem<UGameplayNotifySubsystem>();
	if (!IsValid(Subsystem))
	{
		return;
	}

	FGameplayNotifyDelegate* Delegate = Subsystem->FindNotifyDelegate(NotifyTag);
	if (Delegate)
	{
		Delegate->RemoveAll(this);
	}

	WorldContext.Reset();

	SetReadyToDestroy();
	MarkAsGarbage();
}

