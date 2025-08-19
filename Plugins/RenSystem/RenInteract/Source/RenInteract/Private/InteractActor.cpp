// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InteractActor.h"

// Engine Headers

// Project Headers
#include "RenCoreLibrary/Public/LogMacro.h"

#include "RenCoreInventory/Public/InventoryManagerInterface.h"

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

