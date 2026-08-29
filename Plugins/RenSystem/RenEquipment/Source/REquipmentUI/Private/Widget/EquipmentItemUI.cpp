// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/EquipmentItemUI.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "GameFramework/PlayerController.h"

// Project Headers
#include "EquipmentManagerComponent.h"
#include "Data/CoreDataAsset.h"
#include "System/EquipmentController.h"


UEquipmentManagerComponent* UEquipmentItemUI::GetEquipmentComponent() const
{
	return _EquipmentComponent.Get();
}

UEquipmentController* UEquipmentItemUI::GetEquipmentController() const
{
	return _EquipmentController.Get();
}

void UEquipmentItemUI::RegisterEquipmentComponent(AActor* Target)
{
	if (IsValid(Target))
	{
		UEquipmentManagerComponent* Component = Target->FindComponentByClass<UEquipmentManagerComponent>();
		if (!IsValid(Component))
		{
			return;
		}

		Component->OnEquipmentReset.AddUObject(this, &UEquipmentItemUI::UnregisterEquipmentController);
		Component->OnEquipmentUpdate.AddUObject(this, &UEquipmentItemUI::RegisterEquipmentController);

		_EquipmentComponent = TWeakObjectPtr<UEquipmentManagerComponent>(Component);
	}
}

void UEquipmentItemUI::UnregisterEquipmentComponent()
{
	UEquipmentManagerComponent* Component = GetEquipmentComponent();
	if (IsValid(Component))
	{
		Component->OnEquipmentReset.RemoveAll(this);
		Component->OnEquipmentUpdate.RemoveAll(this);
	}
	_EquipmentComponent.Reset();
}

void UEquipmentItemUI::RegisterEquipmentController()
{
	UEquipmentManagerComponent* Component = GetEquipmentComponent();
	if (!IsValid(Component))
	{
		return;
	}

	UEquipmentController* Controller = Component->GetEquipmentControllerByTag(SlotId);
	if (!IsValid(Controller))
	{
		return;
	}

	_EquipmentController = TWeakObjectPtr<UEquipmentController>(Controller);
	SetDetail(Controller);
}

void UEquipmentItemUI::UnregisterEquipmentController()
{
	ResetDetail();
	_EquipmentController.Reset();
}

void UEquipmentItemUI::SetDetail(UEquipmentController* Controller)
{
	const UCoreDataAsset* Asset = Controller->GetEquipmentAsset();
	if (IsValid(Asset))
	{
		EquipmentImage->SetBrushFromSoftTexture(Asset->Icon);
		EquipmentOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		EquipmentOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UEquipmentItemUI::RefreshDetail()
{
}

void UEquipmentItemUI::ResetDetail()
{
	EquipmentOverlay->SetVisibility(ESlateVisibility::Hidden);
}


void UEquipmentItemUI::RegisterPlayer()
{
	APlayerController* Controller = GetOwningPlayer();
	if (!IsValid(Controller))
	{
		return;
	}

	TWeakObjectPtr<UEquipmentItemUI> WeakThis(this);
	Controller->GetOnNewPawnNotifier().AddWeakLambda(this,
		[WeakThis](APawn* NewPawn)
		{
			UEquipmentItemUI* This = WeakThis.Get();
			if (IsValid(This))
			{
				This->OnPlayerRegistered(NewPawn);
			}
		}
	);

	APawn* ExistingPawn = Controller->GetPawn();
	if (IsValid(ExistingPawn))
	{
		OnPlayerRegistered(ExistingPawn);
	}
}

void UEquipmentItemUI::UnregisterPlayer()
{
	APlayerController* Controller = GetOwningPlayer();
	if (IsValid(Controller))
	{
		Controller->GetOnNewPawnNotifier().RemoveAll(this);
	}

	OnPlayerUnregistered();
}

void UEquipmentItemUI::OnPlayerRegistered(AActor* Target)
{
	OnPlayerUnregistered();

	if (IsValid(Target))
	{
		RegisterEquipmentComponent(Target);
	}
}

void UEquipmentItemUI::OnPlayerUnregistered()
{
	ResetDetail();
	UnregisterEquipmentComponent();
	UnregisterEquipmentController();
}



void UEquipmentItemUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	KeyTextBlock->SetText(KeyText);
	EquipmentOverlay->SetVisibility(ESlateVisibility::Hidden);
}

void UEquipmentItemUI::NativeConstruct()
{
	RegisterPlayer();
	Super::NativeConstruct();
}

void UEquipmentItemUI::NativeDestruct()
{
	UnregisterPlayer();
	Super::NativeDestruct();
}

