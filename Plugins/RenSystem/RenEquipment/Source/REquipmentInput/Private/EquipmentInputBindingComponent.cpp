// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EquipmentInputBindingComponent.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "Engine/AssetManager.h"

// Project Headers
#include "Core/AssetManagerUtil.h"
#include "Data/EquipmentInputBindingAsset.h"
#include "System/Input/EquipmentActionInput.h"
#include "System/Input/EquipmentActivationInput.h"


UEquipmentInputBindingComponent::UEquipmentInputBindingComponent(const FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	bAutoActivate = false;
}

void UEquipmentInputBindingComponent::BeginPlay()
{
	Super::BeginPlay();

	LoadAndRegisterInput();
}

void UEquipmentInputBindingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	FAssetManagerUtil::CancelHandle(AssetHandle);
    UnregisterInput();

	Super::EndPlay(EndPlayReason);
}

#if WITH_EDITOR
EDataValidationResult UEquipmentInputBindingComponent::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	for (const FSoftObjectPath& Path : InputBindings)
	{
		if (Path.IsNull())
		{
			Context.AddError(FText::FromString("Invalid input binding asset in list"));
			return EDataValidationResult::Invalid;
		}
	}

	return Result;
}
#endif

void UEquipmentInputBindingComponent::LoadAndRegisterInput()
{
	FAssetManagerUtil::CancelHandle(AssetHandle);

	FStreamableManager& Manager = UAssetManager::GetStreamableManager();
	AssetHandle = Manager.RequestAsyncLoad(InputBindings, FStreamableDelegate::CreateUObject(this, &UEquipmentInputBindingComponent::HandleOnInputLoaded));
}

void UEquipmentInputBindingComponent::HandleOnInputLoaded()
{
	FAssetManagerUtil::ReleaseHandle(AssetHandle);

	UnregisterInput();
	RegisterInput();
}

void UEquipmentInputBindingComponent::RegisterInput()
{
	for (const FSoftObjectPath& Path : InputBindings)
	{
		UEquipmentInputBindingAsset* Input = Cast<UEquipmentInputBindingAsset>(Path.ResolveObject());
		if (!IsValid(Input))
		{
			continue;
		}

		if (IsValid(Input->ActivationClass))
		{
			UEquipmentActivationInput* Activation = NewObject<UEquipmentActivationInput>(this, Input->ActivationClass);
			Activation->Inputs = Input->ActivationBindings;
			Activation->InitializeHandler();

			InputHandlers.Add(Activation);
		}

		if (IsValid(Input->ActionClass))
		{
			UEquipmentActionInput* Action = NewObject<UEquipmentActionInput>(this, Input->ActionClass);
			Action->Actions = Input->ActionBindings;
			Action->InitializeHandler();

			InputHandlers.Add(Action);
		}
	}
}

void UEquipmentInputBindingComponent::UnregisterInput()
{
	for (UEquipmentInputHandler* Handler : InputHandlers)
	{
		if (IsValid(Handler))
		{
			Handler->DeinitializeHandler();
			Handler->MarkAsGarbage();
		}
	}

	InputHandlers.Empty();
}

