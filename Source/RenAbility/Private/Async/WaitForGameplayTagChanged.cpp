// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Async/WaitForGameplayTagChanged.h"

// Engine Headers
#include "AbilitySystemComponent.h"

// Project Headers
#include "RenCore/Public/Macro/LogMacro.h"



UWaitForGameplayTagChanged* UWaitForGameplayTagChanged::WaitForGameplayTagChanged(AActor* Actor, FGameplayTag Tag)
{
	UWaitForGameplayTagChanged* NewTask = NewObject<UWaitForGameplayTagChanged>();
	NewTask->TargetActor = Actor;
	NewTask->TargetTag = Tag;
	return NewTask;
}

void UWaitForGameplayTagChanged::EndTask()
{
	if (TargetActor.IsValid())
	{
		TargetActor.Reset();
	}

	if (TargetASC.IsValid())
	{
		TargetASC->RegisterGameplayTagEvent(TargetTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		TargetASC.Reset();
	}

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitForGameplayTagChanged::HandleTagChanged(const FGameplayTag Tag, int Count)
{
	OnTagChanged.Broadcast(Count);
}

void UWaitForGameplayTagChanged::Activate()
{
	if (TargetActor.IsValid())
	{
        TargetASC = TargetActor->FindComponentByClass<UAbilitySystemComponent>();
        if (TargetASC.IsValid())
        {
            TargetASC->RegisterGameplayTagEvent(TargetTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UWaitForGameplayTagChanged::HandleTagChanged);
        }
	}
}

