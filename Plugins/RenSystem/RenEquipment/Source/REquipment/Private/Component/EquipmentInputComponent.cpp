// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/EquipmentInputComponent.h"

// Engine Headers
#include "Engine/AssetManager.h"
#include "EnhancedInputComponent.h"

// Project Headers
#include "Core/AssetManagerUtil.h"
#include "Data/EquipmentInputMapping.h"
#include "System/Input/EquipmentActionInput.h"
#include "System/Input/EquipmentActivationInput.h"


UEquipmentInputComponent::UEquipmentInputComponent(const FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UEquipmentInputComponent::Activate(bool bReset)
{
	Super::Activate(bReset);

	LoadAndRegisterInput();
}

void UEquipmentInputComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	FAssetManagerUtil::CancelHandle(_LoadHandle);
    UnregisterInput();

	Super::EndPlay(EndPlayReason);
}

void UEquipmentInputComponent::LoadAndRegisterInput()
{
	FAssetManagerUtil::CancelHandle(_LoadHandle);

	_LoadHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(InputMapping, FStreamableDelegate::CreateUObject(this, &UEquipmentInputComponent::HandleOnInputLoaded));
}

void UEquipmentInputComponent::HandleOnInputLoaded()
{
	FAssetManagerUtil::ReleaseHandle(_LoadHandle);

	UnregisterInput();
	RegisterInput();
}

void UEquipmentInputComponent::RegisterInput()
{
	for (const FSoftObjectPath& Path : InputMapping)
	{
		UEquipmentInputMapping* Input = Cast<UEquipmentInputMapping>(Path.ResolveObject());
		if (!IsValid(Input))
		{
			continue;
		}

		if (IsValid(Input->ActivationClass))
		{
			UEquipmentActivationInput* Activation = NewObject<UEquipmentActivationInput>(this, Input->ActivationClass);
			Activation->Inputs = Input->ActivationBindings;
			Activation->RegisterInput();

			InputHandlers.Add(Activation);
		}

		if (IsValid(Input->ActionClass))
		{
			UEquipmentActionInput* Action = NewObject<UEquipmentActionInput>(this, Input->ActionClass);
			Action->Actions = Input->ActionBindings;
			Action->RegisterInput();

			InputHandlers.Add(Action);
		}
	}
}

void UEquipmentInputComponent::UnregisterInput()
{
	for (UEquipmentInputHandler* Handler : InputHandlers)
	{
		if (IsValid(Handler))
		{
			Handler->UnregisterInput();
			Handler->MarkAsGarbage();
		}
	}

	InputHandlers.Empty();
}

