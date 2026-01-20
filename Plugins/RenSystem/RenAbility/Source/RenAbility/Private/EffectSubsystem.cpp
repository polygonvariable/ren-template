// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EffectSubsystem.h"

// Engine Headers
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"


/*
void UGameplayNotifySubsystem::RegisterListener(FGameplayTag EventTag, UObject* Listener)
{
	if (!IsValid(Listener))
	{
		PRINT_ERROR(LogTemp, 5.0f, TEXT("Listener is not valid"));
		return;
	}
	FGameplayEventListeners& EventListeners = ListenerMap.FindOrAdd(EventTag);
	EventListeners.Listeners.Add(Listener);
}

void UGameplayNotifySubsystem::UnregisterListener(FGameplayTag EventTag, UObject* Listener)
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


void UGameplayNotifySubsystem::SendEvent(const FGameplayTag& EventTag, const FInstancedStruct& Payload)
{
	BroadcastEvent<UGameplayEventInterface>(EventTag, [&](UObject* Listener)
	{
		IGameplayEventInterface::Execute_OnReceiveEvent(Listener, EventTag, Payload);
	});
}

void UGameplayNotifySubsystem::SendScalarEvent(const FGameplayTag& EventTag, const float& Payload)
{
	BroadcastEvent<UGameplayEventExtendedInterface>(EventTag, [&](UObject* Listener)
	{
		IGameplayEventExtendedInterface::Execute_OnReceiveScalarEvent(Listener, EventTag, Payload);
	});
}

void UGameplayNotifySubsystem::SendTextEvent(const FGameplayTag& EventTag, const FText& Payload)
{
	BroadcastEvent<UGameplayEventExtendedInterface>(EventTag, [&](UObject* Listener)
	{
		IGameplayEventExtendedInterface::Execute_OnReceiveTextEvent(Listener, EventTag, Payload);
	});
}



void UGameplayNotifySubsystem::SendGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	OnGameplayEvent.Broadcast(EventTag, *Payload);
}



bool UGameplayNotifySubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
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
*/



bool UGameplayNotifySubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UGameplayNotifySubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	NetworkActor = InWorld.SpawnActor<AGameplayNotifyNetworkActor>();
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






AGameplayNotifyNetworkActor::AGameplayNotifyNetworkActor()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	SetReplicatingMovement(false);
}

void AGameplayNotifyNetworkActor::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		NotifySubsystem = World->GetSubsystem<UGameplayNotifySubsystem>();
	}
}

void AGameplayNotifyNetworkActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	NotifySubsystem.Reset();

	Super::EndPlay(EndPlayReason);
}

void AGameplayNotifyNetworkActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AGameplayNotifyNetworkActor::SendNotifyInternal(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Notify)
{
	UGameplayNotifySubsystem* Subsystem = NotifySubsystem.Get();
	if (IsValid(Subsystem))
	{
		Subsystem->BroadcastNotify(NotifyTag, Notify);
	}
}

void AGameplayNotifyNetworkActor::SendNotify(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Notify, bool bReliable)
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

void AGameplayNotifyNetworkActor::SendNotifyReliable_Implementation(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Notify)
{
	SendNotifyInternal(NotifyTag, Notify);
}

void AGameplayNotifyNetworkActor::SendNotifyUnreliable_Implementation(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Notify)
{
	SendNotifyInternal(NotifyTag, Notify);
}














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

