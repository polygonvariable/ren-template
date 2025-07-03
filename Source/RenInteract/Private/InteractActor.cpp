// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InteractActor.h"

// Engine Headers
#include "GameFramework/HUD.h"

// Project Headers
#include "InteractComponent.h"


AInteractActor::AInteractActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AInteractActor::Interact_Implementation()
{
	OnInteracted();
}

UInteractComponent* AInteractActor::GetInteractComponent_Implementation()
{
	if (IsValid(InteractComponent))
	{
		return InteractComponent;
	}

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController()) 
	{
		if (AHUD* HUD = PlayerController->GetHUD())
		{
			InteractComponent = Cast<UInteractComponent>(HUD->GetComponentByClass(UInteractComponent::StaticClass()));
			return InteractComponent;
		}
	}
	return nullptr;
}

void AInteractActor::StartInteract_Implementation()
{
	if(UInteractComponent* Component = GetInteractComponent())
	{
		Component->AddItem(this);
	}
}

void AInteractActor::EndInteract_Implementation()
{
	if (UInteractComponent* Component = GetInteractComponent())
	{
		Component->RemoveItem(this);
	}
}

void AInteractActor::UpdateItem_Implementation()
{
	OnItemUpdated.Broadcast(InteractItem);
}

void AInteractActor::OnInteracted_Implementation()
{
}

void AInteractActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	EndInteract();
	Super::EndPlay(EndPlayReason);
}

