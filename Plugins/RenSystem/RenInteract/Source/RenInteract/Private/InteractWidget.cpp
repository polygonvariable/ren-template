// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InteractWidget.h"

// Engine Headers
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenInteract/Public/InteractInterface.h"
#include "RenInteract/Public/InteractItem.h"
#include "RenInteract/Public/InteractSubsystem.h"



void UInteractEntryWidget::InitializeDetails(AActor* Actor, const FInteractItem* Item)
{
	IInteractInterface* InteractActor = Cast<IInteractInterface>(Actor);
	if (!InteractActor || !Item)
	{
		LOG_ERROR(LogTemp, TEXT("InteractProvider or Item is invalid"));
		return;
	}

	InteractActor->GetOnInteractUpdated().RemoveAll(this);
	InteractActor->GetOnInteractUpdated().AddUObject(this, &UInteractEntryWidget::HandleItemDetails);

	InteractProvider = InteractActor;

	HandleItemDetails(Item);
}

void UInteractEntryWidget::ResetDetails()
{
	IInteractInterface* InteractActor = InteractProvider.Get();
	if (InteractActor)
	{
		InteractActor->GetOnInteractUpdated().RemoveAll(this);
	}
	InteractProvider.Reset();
}

void UInteractEntryWidget::SelectEntry()
{
	IInteractInterface* InteractActor = InteractProvider.Get();
	if (InteractActor)
	{
		InteractActor->Interacted();
	}
}

void UInteractEntryWidget::HandleItemDetails(const FInteractItem* Item)
{
	if (!Item) return;
	if (InteractTitle) InteractTitle->SetText(Item->InteractTitle);
}

void UInteractEntryWidget::NativeDestruct()
{
	ResetDetails();
	Super::NativeDestruct();
}



void UInteractWidget::HandleInteractStarted(AActor* Actor, const FInteractItem* Item)
{
	if (!IsValid(Actor) || !Item)
	{
		LOG_ERROR(LogTemp, TEXT("Actor or Item is invalid"));
		return;
	}

	if (!InteractEntries.Contains(Actor) && IsValid(InteractEntryClass) && IsValid(InteractEntryPanel))
	{
		UInteractEntryWidget* EntryWidget = CreateWidget<UInteractEntryWidget>(this, InteractEntryClass);
		if (!IsValid(EntryWidget))
		{
			LOG_ERROR(LogTemp, TEXT("InteractEntryWidget is invalid"));
			return;
		}

		InteractEntries.Add(Actor, EntryWidget);

		InteractEntryPanel->AddChild(EntryWidget);

		EntryWidget->InitializeDetails(Actor, Item);
	}
}

void UInteractWidget::HandleInteractEnded(AActor* Actor)
{
	if (!IsValid(Actor))
	{
		LOG_ERROR(LogTemp, TEXT("Actor is invalid"));
		return;
	}
	
	UInteractEntryWidget* EntryWidget = *InteractEntries.Find(Actor);
	if (EntryWidget)
	{
		EntryWidget->ResetDetails();

		InteractEntryPanel->RemoveChild(EntryWidget);
		InteractEntries.Remove(Actor);
	}
}

void UInteractWidget::NativeConstruct()
{
	UInteractSubsystem* InteractSubsystemPtr = GetWorld()->GetSubsystem<UInteractSubsystem>();
	if (!IsValid(InteractSubsystemPtr))
	{
		LOG_ERROR(LogTemp, TEXT("InteractSubsystem is invalid"));
		return;
	}

	InteractSubsystemPtr->OnInteractStarted.AddUObject(this, &UInteractWidget::HandleInteractStarted);
	InteractSubsystemPtr->OnInteractEnded.AddUObject(this, &UInteractWidget::HandleInteractEnded);

	InteractSubsystem = InteractSubsystemPtr;

	Super::NativeConstruct();
}

void UInteractWidget::NativeDestruct()
{
	UInteractSubsystem* InteractSubsystemPtr = InteractSubsystem.Get();
	if (IsValid(InteractSubsystemPtr))
	{
		InteractSubsystemPtr->OnInteractStarted.RemoveAll(this);
		InteractSubsystemPtr->OnInteractEnded.RemoveAll(this);
	}
	InteractSubsystem.Reset();

	InteractEntries.Empty();

	Super::NativeDestruct();
}

