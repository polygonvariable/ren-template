// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InteractSubsystem.h"

// Project Headers
#include "RenCoreLibrary/Public/SubsystemUtils.h"
#include "RenCoreLibrary/Public/LogMacro.h"



void UInteractSubsystem::EnableInteract()
{
	bInteractEnabled = true;
}

void UInteractSubsystem::DisableInteract()
{
	bInteractEnabled = false;
}

void UInteractSubsystem::AddItem(AActor* Actor, const FInteractItem* Item)
{
	if (IsValid(Actor) && bInteractEnabled)
	{
		OnInteractStarted.Broadcast(Actor, Item);
	}
}

void UInteractSubsystem::RemoveItem(AActor* Actor)
{
	if (IsValid(Actor))
	{
		OnInteractEnded.Broadcast(Actor);
	}
}

bool UInteractSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UInteractSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("Initialize InteractSubsystem"));
}

void UInteractSubsystem::Deinitialize()
{
	LOG_WARNING(LogTemp, TEXT("Deinitialize InteractSubsystem"));
	Super::Deinitialize();
}

