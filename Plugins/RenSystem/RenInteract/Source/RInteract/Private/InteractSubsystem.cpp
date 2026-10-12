// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InteractSubsystem.h"

// Project Headers
#include "GameplayModeProvider.h"
#include "InteractComponent.h"
#include "InteractSettings.h"
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

	RegisteredItems.Add(InteractId, TWeakObjectPtr<UInteractComponent>(Interact));
	OnInteractAdded.ExecuteIfBound(InteractId, InteractItem);

	if (RegisteredItems.Num() > 0 && GameplayMode)
	{
		const UInteractSettings* InteractSettings = UInteractSettings::Get();
		GameplayMode->PushGameplayMode(InteractSettings->InteractMode);
	}
}

void UInteractSubsystem::UnregisterItem(const FGuid& InteractId)
{
	if (RegisteredItems.Remove(InteractId) > 0)
	{
		OnInteractRemoved.ExecuteIfBound(InteractId);
	}

	if (RegisteredItems.Num() <= 0 && GameplayMode)
	{
		const UInteractSettings* InteractSettings = UInteractSettings::Get();
		GameplayMode->PopGameplayMode(InteractSettings->InteractMode);
	}
}

void UInteractSubsystem::InteractItemById(const FGuid& InteractId)
{
	TWeakObjectPtr<UInteractComponent>* FoundComponent = RegisteredItems.Find(InteractId);
	if (!FoundComponent)
	{
		LOG_ERROR(LogInteract, TEXT("Interact handle not found"));
		return;
	}

	UInteractComponent* Component = FoundComponent->Get();
	if (!IsValid(Component))
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

bool UInteractSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Super::ShouldCreateSubsystem(Outer) || GetClass() != UInteractSettings::Get()->SubsystemClass)
	{
		return false;
	}
	return true;
}

void UInteractSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	LOG_WARNING(LogInteract, TEXT("InteractSubsystem OnWorldBeginPlay"));

	GameplayMode = FSubsystemLibrary::GetSubsystemInterface<IGameplayModeProvider>(&InWorld);
}

void UInteractSubsystem::OnWorldEndPlay(UWorld& InWorld)
{
	GameplayMode = nullptr;
	RegisteredItems.Empty();

	LOG_WARNING(LogInteract, TEXT("InteractSubsystem OnWorldEndPlay"));
	Super::OnWorldEndPlay(InWorld);
}

UInteractSubsystem* UInteractSubsystem::Get(const UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return World->GetSubsystem<UInteractSubsystem>();
}

