// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EffectSubsystem.h"

// Engine Headers
#include "Kismet/GameplayStatics.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"



void UGameplayEventSubsystem::RegisterListener(FGameplayTag EventTag, UObject* Listener)
{
	if (!IsValid(Listener))
	{
		PRINT_ERROR(LogTemp, 5.0f, TEXT("Listener is not valid"));
		return;
	}
	FGameplayEventListeners& EventListeners = ListenerMap.FindOrAdd(EventTag);
	EventListeners.Listeners.Add(Listener);
}

void UGameplayEventSubsystem::UnregisterListener(FGameplayTag EventTag, UObject* Listener)
{
	if (!IsValid(Listener))
	{
		PRINT_ERROR(LogTemp, 5.0f, TEXT("Listener is not valid"));
		return;
	}

	if (FGameplayEventListeners* EventListeners = ListenerMap.Find(EventTag))
	{
		EventListeners->Listeners.Remove(Listener);
		if (EventListeners->Listeners.Num() == 0)
		{
			ListenerMap.Remove(EventTag);
		}
	}
}


void UGameplayEventSubsystem::SendEvent(const FGameplayTag& EventTag, const FInstancedStruct& Payload)
{
	BroadcastEvent<UGameplayEventInterface>(EventTag, [&](UObject* Listener)
	{
		IGameplayEventInterface::Execute_OnReceiveEvent(Listener, EventTag, Payload);
	});
}

void UGameplayEventSubsystem::SendScalarEvent(const FGameplayTag& EventTag, const float& Payload)
{
	BroadcastEvent<UGameplayEventExtendedInterface>(EventTag, [&](UObject* Listener)
	{
		IGameplayEventExtendedInterface::Execute_OnReceiveScalarEvent(Listener, EventTag, Payload);
	});
}

void UGameplayEventSubsystem::SendTextEvent(const FGameplayTag& EventTag, const FText& Payload)
{
	BroadcastEvent<UGameplayEventExtendedInterface>(EventTag, [&](UObject* Listener)
	{
		IGameplayEventExtendedInterface::Execute_OnReceiveTextEvent(Listener, EventTag, Payload);
	});
}

bool UGameplayEventSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}



void UGameplayEventHandlerSubsystem::RegisterHandler(const FGameplayTag& EventTag, TSubclassOf<UGameplayEventHandler> HandlerClass)
{
	if (!IsValid(HandlerClass))
	{
		PRINT_ERROR(LogTemp, 5.0f, TEXT("HandlerClass is not valid"));
		return;
	}

	UGameplayEventHandler* Handler = NewObject<UGameplayEventHandler>(this, HandlerClass);
	FGameplayEventHandlers& EventHandlers = HandlerMap.FindOrAdd(EventTag);
	EventHandlers.Handlers.Add(Handler);
}

void UGameplayEventHandlerSubsystem::SendEvent(const FGameplayTag& EventTag, const FInstancedStruct& Payload, AActor* Source, AActor* Target)
{
	BroadcastEvent(EventTag, Source, Target, [&](UGameplayEventHandler* Listener)
	{
		Listener->OnReceiveEvent(EventTag, Payload, Source, Target);
	});
}

void UGameplayEventHandlerSubsystem::SendScalarEvent(const FGameplayTag& EventTag, const float& Payload, AActor* Source, AActor* Target)
{
	BroadcastEvent(EventTag, Source, Target, [&](UGameplayEventHandler* Listener)
	{
		PRINT_INFO(LogTemp, 5.0f, TEXT("SendScalarEvent"));
		Listener->OnReceiveScalarEvent(EventTag, Payload, Source, Target);
	});
}

void UGameplayEventHandlerSubsystem::SendTextEvent(const FGameplayTag& EventTag, const FText& Payload, AActor* Source, AActor* Target)
{
	BroadcastEvent(EventTag, Source, Target, [&](UGameplayEventHandler* Listener)
	{
		Listener->OnReceiveTextEvent(EventTag, Payload, Source, Target);
	});
}

bool UGameplayEventHandlerSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

UWorld* UGameplayEventHandler::GetWorld() const
{
	return GetOuter()->GetWorld();
}
