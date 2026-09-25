// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "AvatarInputBindingComponent.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "Engine/AssetManager.h"
#include "EnhancedInputComponent.h"

// Project Headers
#include "Actor/AvatarCharacter.h"
#include "AvatarInputBindingAsset.h"
#include "Core/AssetManagerUtil.h"


UAvatarInputBindingComponent::UAvatarInputBindingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	bAutoActivate = false;
}

void UAvatarInputBindingComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetOwner());
	if (IsValid(PlayerController))
	{
		HandleOnPawnChanged(nullptr, PlayerController->GetPawn());
		PlayerController->OnPossessedPawnChanged.AddDynamic(this, &UAvatarInputBindingComponent::HandleOnPawnChanged);
	}

	FStreamableManager& Manager = UAssetManager::GetStreamableManager();
	AssetHandle = Manager.RequestAsyncLoad(InputBinding.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &UAvatarInputBindingComponent::HandleOnInputAssetLoaded));
}

void UAvatarInputBindingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UEnhancedInputComponent* InputComponent = GetInputComponent();
	if (IsValid(InputComponent))
	{
		for (const uint8& Handle : InputHandles)
		{
			InputComponent->RemoveBindingByHandle(Handle);
		}
		InputHandles.Empty();
	}
	Character = nullptr;

	if (IsValid(PlayerController))
	{
		PlayerController->OnPossessedPawnChanged.RemoveAll(this);
	}
	PlayerController = nullptr;

	FAssetManagerUtil::CancelHandle(AssetHandle);
	InputBindingAsset = nullptr;

	Super::EndPlay(EndPlayReason);
}

#if WITH_EDITOR
EDataValidationResult UAvatarInputBindingComponent::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (InputBinding.IsNull())
	{
		Context.AddError(FText::FromString("Input binding asset is invalid"));
		return EDataValidationResult::Invalid;
	}

	return Result;
}
#endif

UEnhancedInputComponent* UAvatarInputBindingComponent::GetInputComponent() const
{
	if (!IsValid(PlayerController))
	{
		return nullptr;
	}
	return Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
}

void UAvatarInputBindingComponent::HandleOnPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	Character = Cast<AAvatarCharacter>(NewPawn);
}

void UAvatarInputBindingComponent::HandleOnInputAssetLoaded()
{
	FAssetManagerUtil::ReleaseHandle(AssetHandle);
	InputBindingAsset = InputBinding.Get();

	UEnhancedInputComponent* InputComponent = GetInputComponent();
	if (!IsValid(InputBindingAsset) || !IsValid(InputComponent))
	{
		return;
	}

	if (IsValid(InputBindingAsset->CameraPan))
	{
		FEnhancedInputActionEventBinding& CameraPanBinding = InputComponent->BindAction(InputBindingAsset->CameraPan, ETriggerEvent::Triggered, this, &UAvatarInputBindingComponent::HandleOnCameraPan);
		InputHandles.Add(CameraPanBinding.GetHandle());
	}

	if (IsValid(InputBindingAsset->CameraZoom))
	{
		FEnhancedInputActionEventBinding& CameraZoomBinding = InputComponent->BindAction(InputBindingAsset->CameraZoom, ETriggerEvent::Triggered, this, &UAvatarInputBindingComponent::HandleOnCameraZoom);
		InputHandles.Add(CameraZoomBinding.GetHandle());
	}

	if (IsValid(InputBindingAsset->Walk))
	{
		FEnhancedInputActionEventBinding& WalkBinding = InputComponent->BindAction(InputBindingAsset->Walk, ETriggerEvent::Triggered, this, &UAvatarInputBindingComponent::HandleOnWalk);
		InputHandles.Add(WalkBinding.GetHandle());
	}

	if (IsValid(InputBindingAsset->Sprint))
	{
		FEnhancedInputActionEventBinding& SprintBinding = InputComponent->BindAction(InputBindingAsset->Sprint, ETriggerEvent::Triggered, this, &UAvatarInputBindingComponent::HandleOnSprint);
		InputHandles.Add(SprintBinding.GetHandle());
	}

	if (IsValid(InputBindingAsset->Jump))
	{
		FEnhancedInputActionEventBinding& JumpBinding = InputComponent->BindAction(InputBindingAsset->Jump, ETriggerEvent::Triggered, this, &UAvatarInputBindingComponent::HandleOnJump);
		InputHandles.Add(JumpBinding.GetHandle());
	}
}

void UAvatarInputBindingComponent::HandleOnCameraPan(const FInputActionValue& Value)
{
	if (IsValid(Character) && Value.GetValueType() == EInputActionValueType::Axis2D)
	{
		FInputActionValue::Axis2D Axis = Value.Get<FVector2D>();

		Character->CameraPan(Axis);
	}
}

void UAvatarInputBindingComponent::HandleOnCameraZoom(const FInputActionValue& Value)
{
	if (IsValid(Character) && Value.GetValueType() == EInputActionValueType::Axis1D)
	{
		FInputActionValue::Axis1D Delta = Value.Get<float>();

		Character->CameraZoom(Delta * 20);
	}
}

void UAvatarInputBindingComponent::HandleOnWalk(const FInputActionValue& Value)
{
	if (IsValid(Character) && Value.GetValueType() == EInputActionValueType::Axis2D)
	{
		FInputActionValue::Axis2D Axis = Value.Get<FVector2D>();

		Character->DirectionalMove(FVector(Axis.X, Axis.Y, 0.0f));
	}
}

void UAvatarInputBindingComponent::HandleOnSprint(const FInputActionValue& Value)
{
}

void UAvatarInputBindingComponent::HandleOnJump(const FInputActionValue& Value)
{
	if (IsValid(Character) && Value.GetValueType() == EInputActionValueType::Boolean)
	{
		Character->Jump();
	}
}

