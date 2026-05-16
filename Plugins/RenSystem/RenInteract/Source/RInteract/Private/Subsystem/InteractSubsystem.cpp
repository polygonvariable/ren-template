// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/InteractSubsystem.h"

// Project Headers
#include "Interface/InteractInterface.h"
#include "Log/LogMacro.h"


void UInteractSubsystem::RegisterItem(const FGuid& InteractId, AActor* Actor, const FInteractItem& InteractItem)
{
	if (!InteractId.IsValid() || !IsValid(Actor) || !Actor->Implements<UInteractInterface>())
	{
		LOG_ERROR(LogTemp, TEXT("InteractId, Actor is invalid or Actor doesn't implement IInteractInterface"));
		return;
	}

	RegisteredItems.Add(InteractId, TPair<TWeakObjectPtr<AActor>, FInteractItem>(Actor, InteractItem));
	OnInteractAdded.ExecuteIfBound(InteractId, InteractItem);
}

void UInteractSubsystem::UnregisterItem(const FGuid& InteractId)
{
	if (RegisteredItems.Remove(InteractId) > 0)
	{
		OnInteractRemoved.ExecuteIfBound(InteractId);
	}
}

void UInteractSubsystem::InteractItemById(const FGuid& InteractId)
{
	const TPair<TWeakObjectPtr<AActor>, FInteractItem>* Handle = RegisteredItems.Find(InteractId);
	if (!Handle)
	{
		LOG_ERROR(LogTemp, TEXT("Interact handle not found"));
		return;
	}

	AActor* Actor = Handle->Key.Get();
	IInteractInterface* InteractInterface = Cast<IInteractInterface>(Actor);
	if (!InteractInterface)
	{
		LOG_ERROR(LogTemp, TEXT("Actor is invalid or Actor doesn't implement IInteractInterface"));
		return;
	}

	InteractInterface->OnInteracted();
}

bool UInteractSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UInteractSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("InteractSubsystem initialized"));
}

void UInteractSubsystem::Deinitialize()
{
	RegisteredItems.Empty();

	LOG_WARNING(LogTemp, TEXT("InteractSubsystem deinitialized"));
	Super::Deinitialize();
}

UInteractSubsystem* UInteractSubsystem::Get(const UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return World->GetSubsystem<UInteractSubsystem>();
}

