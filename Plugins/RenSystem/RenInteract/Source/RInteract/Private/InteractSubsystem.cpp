// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InteractSubsystem.h"

// Project Headers
#include "GameplayModeProvider.h"
#include "InteractComponent.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "SubsystemLibrary.h"


void UInteractSubsystem::RegisterItem(const FGuid& InteractId, UInteractComponent* Interact, const FInteractItem& InteractItem)
{
	if (!InteractId.IsValid() || !IsValid(Interact))
	{
		LOG_ERROR(LogInteract, TEXT("InteractId, interact component is invalid"));
		return;
	}

	RegisteredItems.Add(InteractId, TPair<TWeakObjectPtr<UInteractComponent>, FInteractItem>(Interact, InteractItem));
	OnInteractAdded.ExecuteIfBound(InteractId, InteractItem);

	if (RegisteredItems.Num() > 0 && !bSetMode)
	{
		IGameplayModeProvider* GameplayMode = FSubsystemLibrary::GetSubsystemInterface<IGameplayModeProvider>(GetWorld());
		if (GameplayMode)
		{
			GameplayMode->PushGameplayMode(TEXT("Interact"));
			bSetMode = true;
		}
	}
}

void UInteractSubsystem::UnregisterItem(const FGuid& InteractId)
{
	if (RegisteredItems.Remove(InteractId) > 0)
	{
		OnInteractRemoved.ExecuteIfBound(InteractId);
	}

	if (RegisteredItems.Num() <= 0 && bSetMode)
	{
		IGameplayModeProvider* GameplayMode = FSubsystemLibrary::GetSubsystemInterface<IGameplayModeProvider>(GetWorld());
		if (GameplayMode)
		{
			GameplayMode->PopGameplayMode(TEXT("Interact"));
			bSetMode = false;
		}
	}
}

void UInteractSubsystem::InteractItemById(const FGuid& InteractId)
{
	const TPair<TWeakObjectPtr<UInteractComponent>, FInteractItem>* FoundHandle = RegisteredItems.Find(InteractId);
	if (!FoundHandle)
	{
		LOG_ERROR(LogInteract, TEXT("Interact handle not found"));
		return;
	}

	UInteractComponent* Component = FoundHandle->Key.Get();
	if (!Component)
	{
		LOG_ERROR(LogInteract, TEXT("Interact component is invalid"));
		return;
	}

	Component->OnInteracted();
}

bool UInteractSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UInteractSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogInteract, TEXT("InteractSubsystem initialized"));
}

void UInteractSubsystem::Deinitialize()
{
	RegisteredItems.Empty();

	LOG_WARNING(LogInteract, TEXT("InteractSubsystem deinitialized"));
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

