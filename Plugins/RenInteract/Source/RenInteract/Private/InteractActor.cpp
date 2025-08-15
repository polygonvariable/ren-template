// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InteractActor.h"

// Engine Headers

// Project Headers
#include "RenCore/Public/EventTimestamp/EventTimestampManagerInterface.h"
#include "RenCoreInventory/Public/InventoryManagerInterface.h"
#include "RenCore/Public/Library/MiscLibrary.h"
#include "RenCore/Public/Macro/LogMacro.h"

#include "RenInteract/Public/InteractSubsystem.h"



AInteractActor::AInteractActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AInteractActor::Interacted()
{
	BP_Interacted();
	OnInteracted.Broadcast();
}

void AInteractActor::HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DoesCollidedWithPlayer(OtherActor) && !bPlayerInRegion)
	{
		bPlayerInRegion = true;
		StartInteract();
	}
}

void AInteractActor::HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	if (DoesCollidedWithPlayer(OtherActor) && bPlayerInRegion)
	{
		bPlayerInRegion = false;
		EndInteract();
	}
}

void AInteractActor::StartInteract()
{
	UInteractSubsystem* InteractSubsystem = GetWorld()->GetSubsystem<UInteractSubsystem>();
	if (IsValid(InteractSubsystem))
	{
		InteractSubsystem->AddItem(this, &InteractItem);
	}
}

void AInteractActor::EndInteract()
{
	UInteractSubsystem* InteractSubsystem = GetWorld()->GetSubsystem<UInteractSubsystem>();
	if (IsValid(InteractSubsystem))
	{
		InteractSubsystem->RemoveItem(this);
	}
}

void AInteractActor::UpdateInteract()
{
	OnInteractUpdated.Broadcast(&InteractItem);
}

FOnInteracted& AInteractActor::GetOnInteracted()
{
	return OnInteracted;
}

FOnInteractUpdated& AInteractActor::GetOnInteractUpdated()
{
	return OnInteractUpdated;
}

void AInteractActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	EndInteract();

	Super::EndPlay(EndPlayReason);
}




void AInventoryPickupActor::Interacted()
{
	IInventoryManagerInterface* InventoryManager = SubsystemUtils::GetSubsystemInterface<IInventoryManagerInterface>(GetWorld()->GetGameInstance());
	if (!InventoryManager)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryManager is invalid"));
		return;
	}

	if (InventoryManager->AddItem(ContainerId, InventoryAsset, ItemQuantity))
	{
		Super::Interacted();
	}
}





AInteractCooldownActor::AInteractCooldownActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void AInteractCooldownActor::Interacted()
{
	IEventTimestampManagerInterface* EventTimestampManager = EventTimestampInterface.Get();
	if (!EventTimestampManager)
	{
		LOG_ERROR(LogTemp, TEXT("EventTimestampManager is invalid"));
		return;
	}

	EventTimestampManager->AddHistory(FName(*CooldownGuid.ToString()), bOnlyOnce);

	Super::Interacted();
}

void AInteractCooldownActor::HandleEventCooldownStatus_Implementation(EEventTimestampCooldownStatus Status)
{
}

void AInteractCooldownActor::BeginPlay()
{
	IEventTimestampManagerInterface* EventTimestampManager = SubsystemUtils::GetSubsystemInterface<IEventTimestampManagerInterface>(GetWorld());
	if (!EventTimestampManager)
	{
		LOG_ERROR(LogTemp, TEXT("EventTimestampManager is invalid"));
		return;
	}
	
	EEventTimestampCooldownStatus Status = EventTimestampManager->GetHistoryCooldownStatus(FName(*CooldownGuid.ToString()), bOnlyOnce, CooldownDuration);
	HandleEventCooldownStatus(Status);

	EventTimestampInterface = EventTimestampManager;

	Super::BeginPlay();
}

void AInteractCooldownActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	EventTimestampInterface.Reset();

	Super::EndPlay(EndPlayReason);
}


