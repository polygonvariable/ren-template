// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Async/WaitForAnyGameplayTagChanged.h"

// Engine Headers
#include "AbilitySystemComponent.h"

// Project Headers
#include "RenCoreLibrary/Public/LogMacro.h"



UWaitForAnyGameplayTagChanged* UWaitForAnyGameplayTagChanged::WaitForAnyGameplayTagChanged(AActor* Actor)
{
	UWaitForAnyGameplayTagChanged* NewTask = NewObject<UWaitForAnyGameplayTagChanged>();
	NewTask->TargetActor = Actor;
	return NewTask;
}

void UWaitForAnyGameplayTagChanged::EndTask()
{
	if (TargetActor.IsValid())
	{
		TargetActor.Reset();
	}

	if (TargetASC.IsValid())
	{
		TargetASC->RegisterGenericGameplayTagEvent().RemoveAll(this);
		TargetASC.Reset();
	}

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitForAnyGameplayTagChanged::HandleTagChanged(const FGameplayTag Tag, int Count)
{
	OnTagChanged.Broadcast(Tag, Count);
}

void UWaitForAnyGameplayTagChanged::Activate()
{
	if (TargetActor.IsValid())
	{
        TargetASC = TargetActor->FindComponentByClass<UAbilitySystemComponent>();
        if (TargetASC.IsValid())
        {
            TargetASC->RegisterGenericGameplayTagEvent().AddUObject(this, &UWaitForAnyGameplayTagChanged::HandleTagChanged);
        }
	}
}

