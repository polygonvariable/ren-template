// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Ability/WeaponAbility_Gun.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Components/SceneComponent.h"
#include "EquipmentActor.h"
#include "GameplayTagAssetInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "System/EquipmentController.h"


UWeaponAbility_Gun::UWeaponAbility_Gun()
{
	ProjectileSocket = TEXT("ProjectileSocket");
	AimTag = FGameplayTag::RequestGameplayTag(TEXT("Equipment.Aim"));
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}


#if WITH_EDITOR
EDataValidationResult UWeaponAbility_Gun::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (!IsValid(SingleFireMontage))
	{
		Context.AddError(FText::FromString("Single fire montage is invalid"));
		return EDataValidationResult::Invalid;
	}

	if (!IsValid(HoldFireMontage))
	{
		Context.AddError(FText::FromString("Hold fire montage is invalid"));
		return EDataValidationResult::Invalid;
	}

	if (!ProjectileSocket.IsValid())
	{
		Context.AddError(FText::FromString("Projectile socket name is invalid"));
		return EDataValidationResult::Invalid;
	}

	if (!AimTag.IsValid())
	{
		Context.AddError(FText::FromString("Aim tag is invalid"));
		return EDataValidationResult::Invalid;
	}

	return Result;
}
#endif


void UWeaponAbility_Gun::OnInputSingle()
{
	if (!IsActive())
	{
		return;
	}

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, SingleFireMontage, 1.0f, NAME_None, false);
	MontageTask->ReadyForActivation();

	TraceProjectile();
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UWeaponAbility_Gun::OnInputHold()
{
	if (!IsActive())
	{
		return;
	}

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, HoldFireMontage, 1.0f, NAME_None, true);
	MontageTask->ReadyForActivation();

	HandleOnLockFinished();
}


void UWeaponAbility_Gun::HandleOnLockFinished()
{
	LockTask = UAbilityTask_WaitDelay::WaitDelay(this, HoldFireInterval);
	LockTask->OnFinish.AddDynamic(this, &UWeaponAbility_Gun::HandleOnLockFinished);
	LockTask->ReadyForActivation();

	TraceProjectile();
}

void UWeaponAbility_Gun::TraceProjectile()
{
	UEquipmentController* Controller = Cast<UEquipmentController>(GetCurrentSourceObject());
	if (!IsValid(Controller))
	{
		return;
	}

	IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(GetAvatarActorFromActorInfo());
	if (!TagInterface)
	{
		return;
	}

	FVector TraceStart(0, 0, 0);
	FVector TraceEnd(0, 0, 0);

	if (TagInterface->HasMatchingGameplayTag(AimTag))
	{
		APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		if (IsValid(CameraManager))
		{
			FVector Location = CameraManager->GetCameraLocation();
			FVector RightVector = CameraManager->GetActorRightVector();
			FVector UpVector = CameraManager->GetActorUpVector();
			FVector ForwardVector = CameraManager->GetActorForwardVector();

			TraceStart = Location + (RightVector * AimOffset.X) + (UpVector * AimOffset.Y);
			TraceEnd = TraceStart + (ForwardVector * TraceDistance);
		}
	}
	else
	{
		AEquipmentActor* Actor = Controller->GetEquipmentActor();
		if (IsValid(Actor))
		{
			USceneComponent* SocketComponent = Actor->FindComponentByTag<USceneComponent>(ProjectileSocket);
			if (IsValid(SocketComponent))
			{
				FVector Location = SocketComponent->GetComponentLocation();
				FVector ForwardVector = SocketComponent->GetForwardVector();

				TraceStart = Location;
				TraceEnd = TraceStart + (ForwardVector * TraceDistance);
			}
		}
	}

	FHitResult OutHit;
	bool bHitResult = UKismetSystemLibrary::LineTraceSingleByProfile(GetWorld(), TraceStart, TraceEnd, TEXT_EMPTY, false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, OutHit, true);
	if (!bHitResult)
	{
		return;
	}
}

