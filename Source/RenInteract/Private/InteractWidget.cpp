// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InteractWidget.h"

// Engine Headers
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/HUD.h"

// Project Headers
#include "InteractActor.h"
#include "InteractComponent.h"
#include "RenCore/Public/Macro/LogMacro.h"


bool UInteractEntryWidget::InitializeEntry_Implementation(AActor* Actor)
{
	if (!IsValid(Actor))
	{
		LOG_ERROR(LogTemp, "Actor is not valid");
		return false;
	}

	InteractActor = Cast<AInteractActor>(Actor);
	if (!IsValid(InteractActor))
	{
		LOG_ERROR(LogTemp, "Actor is not type of InteractActor");
		return false;
	}

	InteractActor->OnItemUpdated.RemoveDynamic(this, &UInteractEntryWidget::OnItemUpdated);
	InteractActor->OnItemUpdated.AddDynamic(this, &UInteractEntryWidget::OnItemUpdated);

	OnItemUpdated(InteractActor->InteractItem);

	return true;
}

void UInteractEntryWidget::SelectEntry_Implementation()
{
	if (IsValid(InteractActor))
	{
		InteractActor->Interact();
	}
}

void UInteractEntryWidget::OnItemUpdated_Implementation(FInteractItem Item)
{
	InteractItem = Item;
	if (InteractTitle) InteractTitle->SetText(Item.Name);
	LOG_WARNING(LogTemp, "InteractItem: %s", *Item.Name.ToString());
}

void UInteractEntryWidget::NativeDestruct()
{
	if (IsValid(InteractActor))
	{
		InteractActor->OnItemUpdated.RemoveDynamic(this, &UInteractEntryWidget::OnItemUpdated);
	}
}




void UInteractWidget::InitializeInteract_Implementation()
{
	HandleComponentBinding(false);
}

void UInteractWidget::DeinitializeInteract_Implementation()
{
	HandleComponentBinding(true);
}

void UInteractWidget::HandleComponentBinding_Implementation(bool bIsUnbind)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!IsValid(PlayerController))
	{
		LOG_ERROR(LogTemp, "PlayerController is not valid");
		return;
	}

	AHUD* HUD = PlayerController->GetHUD();
	if (!IsValid(HUD))
	{
		LOG_ERROR(LogTemp, "HUD is not valid");
		return;
	}

	UInteractComponent* InteractComponent = Cast<UInteractComponent>(HUD->GetComponentByClass(UInteractComponent::StaticClass()));
	if (!IsValid(InteractComponent))
	{
		LOG_ERROR(LogTemp, "InteractComponent is not valid");
		return;
	}

	if (!bIsUnbind)
	{
		if (!InteractComponent->OnInteractStarted.IsBound() && !InteractComponent->OnInteractEnded.IsBound())
		{
			InteractComponent->OnInteractStarted.AddDynamic(this, &UInteractWidget::OnInteractStarted);
			InteractComponent->OnInteractEnded.AddDynamic(this, &UInteractWidget::OnInteractEnded);
		}
	}
	else {
		InteractComponent->OnInteractStarted.RemoveDynamic(this, &UInteractWidget::OnInteractStarted);
		InteractComponent->OnInteractEnded.RemoveDynamic(this, &UInteractWidget::OnInteractEnded);
	}
}

void UInteractWidget::AddItem_Implementation(AActor* Actor)
{
	if (!InteractEntries.Contains(Actor))
	{
		if (IsValid(InteractEntryClass) && IsValid(InteractEntryPanel))
		{
			UInteractEntryWidget* InteractEntryWidget = CreateWidget<UInteractEntryWidget>(GetWorld(), InteractEntryClass);
			if (!IsValid(InteractEntryWidget))
			{
				LOG_ERROR(LogTemp, "Failed to create InteractEntryWidget");
				return;
			}
			InteractEntries.Add(Actor, InteractEntryWidget);
			InteractEntryPanel->AddChild(InteractEntryWidget);
			InteractEntryWidget->InitializeEntry(Actor);
		}
	}
}

void UInteractWidget::RemoveItem_Implementation(AActor* Actor)
{
	if (UInteractEntryWidget* InteractEntryWidget = *InteractEntries.Find(Actor))
	{
		InteractEntryPanel->RemoveChild(InteractEntryWidget);
		InteractEntries.Remove(Actor);
	}
}


void UInteractWidget::OnInteractStarted_Implementation(AActor* Actor)
{
	if (IsValid(Actor))
	{
		AddItem(Actor);
	}
}

void UInteractWidget::OnInteractEnded_Implementation(AActor* Actor)
{
	if (IsValid(Actor))
	{
		RemoveItem(Actor);
	}
}

void UInteractWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeInteract();
}

void UInteractWidget::NativeDestruct()
{
	DeinitializeInteract();
	Super::NativeDestruct();
}

