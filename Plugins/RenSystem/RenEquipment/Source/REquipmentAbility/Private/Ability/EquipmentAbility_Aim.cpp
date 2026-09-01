// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Ability/EquipmentAbility_Aim.h"

// Engine Headers
#include "Abilities/Tasks/AbilityTask_Repeat.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


UEquipmentAbility_Aim::UEquipmentAbility_Aim()
{
	SetAssetTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Equipment.Aim"))));

	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Equipment.Aim")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Equipment.Aim")));

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEquipmentAbility_Aim::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!IsValid(Character))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	USpringArmComponent* SpringArm = GetSprintArmComponent(Character);
	if (IsValid(SpringArm))
	{
		_PreviousSocketLength = SpringArm->TargetArmLength;
		_PreviousSocketOffset = SpringArm->SocketOffset;

		SpringArm->TargetArmLength = SocketLength;
		SpringArm->SocketOffset = SocketOffset;
		SpringArm->bEnableCameraRotationLag = false;
		SpringArm->bEnableCameraLag = false;
	}

	UCharacterMovementComponent* CMC = Character->GetCharacterMovement();
	CMC->bUseControllerDesiredRotation = true;
	CMC->bOrientRotationToMovement = false;

	UCameraComponent* Camera = GetCameraComponent(Character);
	if (IsValid(Camera))
	{
		_PreviousFieldOfView = Camera->FieldOfView;

		Camera->SetFieldOfView(FieldOfView);
	}

	AimHandle = ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, AimEffectClass->GetDefaultObject<UGameplayEffect>(), 1);

	UAbilityTask_WaitInputRelease* InputTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	InputTask->OnRelease.AddDynamic(this, &UEquipmentAbility_Aim::HandleOnInputReleased);
	InputTask->ReadyForActivation();
}

void UEquipmentAbility_Aim::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (IsValid(Character))
	{
		USpringArmComponent* SpringArm = GetSprintArmComponent(Character);
		if (IsValid(SpringArm))
		{
			SpringArm->TargetArmLength = _PreviousSocketLength;
			SpringArm->SocketOffset = _PreviousSocketOffset;
			SpringArm->bEnableCameraRotationLag = true;
			SpringArm->bEnableCameraLag = true;
		}

		UCharacterMovementComponent* CMC = Character->GetCharacterMovement();
		CMC->bUseControllerDesiredRotation = false;
		CMC->bOrientRotationToMovement = true;

		UCameraComponent* Camera = GetCameraComponent(Character);
		if (IsValid(Camera))
		{
			Camera->SetFieldOfView(_PreviousFieldOfView);
		}

		UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
		if (IsValid(ASC))
		{
			ASC->RemoveActiveGameplayEffect(AimHandle);
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

USpringArmComponent* UEquipmentAbility_Aim::GetSprintArmComponent_Implementation(ACharacter* Character) const
{
	return nullptr;
}

UCameraComponent* UEquipmentAbility_Aim::GetCameraComponent_Implementation(ACharacter* Character) const
{
	return Character->GetComponentByClass<UCameraComponent>();
}

void UEquipmentAbility_Aim::HandleOnInputReleased(float TimeHeld)
{
	if (IsActive())
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}

