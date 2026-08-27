// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Controller/EquipmentStateController_Attachment.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

// Project Headers
#include "Actor/EquipmentActor.h"
#include "Core/EquipmentSettings.h"
#include "Data/EquipmentDataDefinition.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


bool UEquipmentAttachmentController::ActivateEquipment()
{
	if (GetState() == EEquipmentState::Inactive && CanActivate())
	{
		SetState(EEquipmentState::Activating);
		LinkAnimInstance();
		PlayEquipAnimation();
		return true;
	}
	return false;
}

bool UEquipmentAttachmentController::DeactivateEquipment(bool bForce)
{
	if (GetState() == EEquipmentState::Inactive)
	{
		return false;
	}

	if (bForce)
	{
		SetState(EEquipmentState::Deactivating);
		CompleteDeactivation();
		return true;
	}

	if (GetState() == EEquipmentState::Active)
	{
		SetState(EEquipmentState::Deactivating);
		PlayUnequipAnimation();
		return true;
	}

	return false;
}


#if WITH_EDITOR
EDataValidationResult UEquipmentAttachmentController::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);
	if ((IsValid(EquipAnimation) || IsValid(UnequipAnimation)) && !IsValid(EquipmentAnimInstance))
	{
		Result = EDataValidationResult::Invalid;
		Context.AddError(FText::FromString("For equipping and unequipping animations, EquipmentAnimInstance is required"));
	}

	return Result;
}
#endif


void UEquipmentAttachmentController::LinkAnimInstance()
{
	UAnimInstance* AnimInstance = GetOwnerAnimInstance();
	if (IsValid(AnimInstance) && IsValid(EquipmentAnimInstance))
	{
		AnimInstance->LinkAnimClassLayers(EquipmentAnimInstance);
	}
}

void UEquipmentAttachmentController::UnlinkAnimInstance()
{
	UAnimInstance* AnimInstance = GetOwnerAnimInstance();
	if (IsValid(AnimInstance) && IsValid(EquipmentAnimInstance))
	{
		AnimInstance->UnlinkAnimClassLayers(EquipmentAnimInstance);
	}
}


void UEquipmentAttachmentController::PlayEquipAnimation()
{
	bAttachNotifyReceived = false;

	UAnimInstance* AnimInstance = GetOwnerAnimInstance();
	if (!IsValid(AnimInstance) || !IsValid(EquipAnimation))
	{
		LOG_ERROR(LogEquipment, TEXT("Anim instance, equip animation is invalid or is not initialized"));
		CompleteActivation();
		return;
	}

	float PlayDuration = AnimInstance->Montage_Play(EquipAnimation, 1.0f);
	if (PlayDuration <= 0.0f)
	{
		LOG_ERROR(LogEquipment, TEXT("Failed to play equip animation"));
		CompleteActivation();
		return;
	}

	RegisterAttachmentNotify(true);
	RegisterMontageEndDelegate(AnimInstance, EquipAnimation, true);
}

void UEquipmentAttachmentController::PlayUnequipAnimation()
{
	bDetachNotifyReceived = false;

	UAnimInstance* AnimInstance = GetOwnerAnimInstance();
	if (!IsValid(AnimInstance) || !IsValid(UnequipAnimation))
	{
		LOG_ERROR(LogEquipment, TEXT("Anim instance, unequip animation is invalid or is not initialized"));
		CompleteDeactivation();
		return;
	}

	float PlayDuration = AnimInstance->Montage_Play(UnequipAnimation, 1.0f);
	if (PlayDuration <= 0.0f)
	{
		LOG_ERROR(LogEquipment, TEXT("Failed to play unequip animation"));
		CompleteDeactivation();
		return;
	}

	RegisterAttachmentNotify(false);
	RegisterMontageEndDelegate(AnimInstance, UnequipAnimation, false);
}


void UEquipmentAttachmentController::RegisterMontageEndDelegate(UAnimInstance* AnimInstance, UAnimMontage* Montage, bool bIsEquip)
{
	FOnMontageEnded Delegate;
	if (bIsEquip)
	{
		Delegate.BindUObject(this, &UEquipmentAttachmentController::HandleOnMontageEquipEnded);
	}
	else
	{
		Delegate.BindUObject(this, &UEquipmentAttachmentController::HandleOnMontageUnequipEnded);
	}
	AnimInstance->Montage_SetEndDelegate(Delegate, Montage);
}

void UEquipmentAttachmentController::RegisterAttachmentNotify(bool bIsEquip)
{
	UAbilitySystemComponent* AbilitySystem = GetOwnerAbilitySystemComponent();
	if (IsValid(AbilitySystem))
	{
		const FGameplayTag& AttachmentTag = UEquipmentSettings::GetAttachmentEventTag(bIsEquip);

		FGameplayEventMulticastDelegate& Delegate = AbilitySystem->GenericGameplayEventCallbacks.FindOrAdd(AttachmentTag);
		Delegate.AddUObject(this, &UEquipmentAttachmentController::HandleOnMontageAttachmentNotify, bIsEquip);
	}
}

void UEquipmentAttachmentController::UnregisterAttachmentNotify(bool bIsEquip)
{
	UAbilitySystemComponent* AbilitySystem = GetOwnerAbilitySystemComponent();
	if (IsValid(AbilitySystem))
	{
		const FGameplayTag& AttachmentTag = UEquipmentSettings::GetAttachmentEventTag(bIsEquip);

		FGameplayEventMulticastDelegate& Delegate = AbilitySystem->GenericGameplayEventCallbacks.FindOrAdd(AttachmentTag);
		Delegate.RemoveAll(this);
	}
}


void UEquipmentAttachmentController::HandleOnMontageAttachmentNotify(const FGameplayEventData* Payload, bool bIsEquip)
{
	UnregisterAttachmentNotify(bIsEquip);

	if (bIsEquip)
	{
		AttachToEquipSocket();
		bAttachNotifyReceived = true;
	}
	else
	{
		AttachToUnequipSocket();
		bDetachNotifyReceived = true;
	}
}

void UEquipmentAttachmentController::HandleOnMontageEquipEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UnregisterAttachmentNotify(true);
	CompleteActivation();
}

void UEquipmentAttachmentController::HandleOnMontageUnequipEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UnregisterAttachmentNotify(false);
	CompleteDeactivation();
}


void UEquipmentAttachmentController::AttachToEquipSocket()
{
	const UEquipmentDataDefinition_Weapon* WeaponDefinition = Cast<UEquipmentDataDefinition_Weapon>(DataDefinition);
	if (IsValid(WeaponDefinition))
	{
		const FEquipmentSlotData* SlotData = UEquipmentSettings::GetEquipmentSlotById(EquipmentData.SlotId);
		if (SlotData)
		{
			AttachToSocket(SlotData->AttachSocket);
		}
	}
}

void UEquipmentAttachmentController::AttachToUnequipSocket()
{
	const UEquipmentDataDefinition_Weapon* WeaponDefinition = Cast<UEquipmentDataDefinition_Weapon>(DataDefinition);
	if (IsValid(WeaponDefinition))
	{
		const FEquipmentSlotData* SlotData = UEquipmentSettings::GetEquipmentSlotById(EquipmentData.SlotId);
		if (SlotData)
		{
			AttachToSocket(SlotData->DetachSocket);
		}
	}
}

void UEquipmentAttachmentController::AttachToSocket(const FEquipmentSocketInfo& Socket)
{
	ACharacter* Character = GetEquipmentOwner<ACharacter>();
	if (IsValid(Character))
	{
		FName SocketName = Socket.SocketName;
		FTransform SocketTransform = Socket.SocketTransform;

		USceneComponent* TargetComponent = Character->GetMesh();
		if (Socket.bUseComponent)
		{
			TargetComponent = Character->FindComponentByTag<USceneComponent>(SocketName);
		}

		EquipmentActor->AttachToComponent(TargetComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		EquipmentActor->SetActorRelativeTransform(SocketTransform);
	}
}


void UEquipmentAttachmentController::AttachEquipment()
{
	if (IsValid(EquipmentActor) && !IsAttached())
	{
		AttachToUnequipSocket();
		SetIsAttached(true);
	}
}

void UEquipmentAttachmentController::CompleteActivation()
{
	UnregisterAttachmentNotify(true);

	if (GetState() == EEquipmentState::Activating)
	{
		if (!bAttachNotifyReceived)
		{
			AttachToEquipSocket();
		}

		bAttachNotifyReceived = false;
		SetState(EEquipmentState::Active);

		OnActivatedDelegate.ExecuteIfBound(this);
	}
}

void UEquipmentAttachmentController::CompleteDeactivation()
{
	UnregisterAttachmentNotify(true);
	UnregisterAttachmentNotify(false);

	if (GetState() == EEquipmentState::Deactivating)
	{
		if (!bDetachNotifyReceived)
		{
			AttachToUnequipSocket();
		}

		bAttachNotifyReceived = false;
		bDetachNotifyReceived = false;

		UnlinkAnimInstance();
		SetState(EEquipmentState::Inactive);

		OnDeactivatedDelegate.ExecuteIfBound(this);
	}
}

