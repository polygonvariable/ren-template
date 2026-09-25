// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/AvatarCharacter.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/AssetManager.h"
#include "EnhancedInputComponent.h"

// Project Headers
#include "Core/AvatarSettings.h"
#include "Core/CharacterSettings.h"
#include "Data/CharacterAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "System/AvatarStorageManager.h"
#include "System/AvatarSubsystem.h"
#include "Util/SubsystemUtil.h"
#include "Core/AssetManagerUtil.h"


AAvatarCharacter::AAvatarCharacter() : Super()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	if (IsValid(SpringArm))
	{
		SpringArm->SetupAttachment(GetRootComponent());
		SpringArm->TargetArmLength = 400.f;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritRoll = false;
		SpringArm->bEnableCameraLag = true;
		SpringArm->bEnableCameraRotationLag = true;
		SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));

		Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
		if (IsValid(Camera))
		{
			Camera->SetupAttachment(SpringArm);
		}
	}

	WeaponSocket01 = CreateDefaultSubobject<USpringArmComponent>(TEXT("Weapon01"));
	if (IsValid(WeaponSocket01))
	{
		WeaponSocket01->SetupAttachment(GetMesh(), TEXT("thigh_out_lSocket"));
		WeaponSocket01->TargetArmLength = 2.0f;
		WeaponSocket01->bEnableCameraLag = true;
		WeaponSocket01->bEnableCameraRotationLag = true;
		//WeaponSocket01->bDrawDebugLagMarkers = true;
		WeaponSocket01->CameraLagSpeed = 35.0f;
		WeaponSocket01->CameraRotationLagSpeed = 15.0f;
		WeaponSocket01->ComponentTags.Add(TEXT("Weapon01"));
	}

	WeaponSocket02 = CreateDefaultSubobject<USpringArmComponent>(TEXT("Weapon02"));
	if (IsValid(WeaponSocket02))
	{
		WeaponSocket02->SetupAttachment(GetMesh(), TEXT("thigh_out_rSocket"));
		WeaponSocket02->TargetArmLength = 2.0f;
		WeaponSocket02->bEnableCameraLag = true;
		WeaponSocket02->bEnableCameraRotationLag = true;
		//WeaponSocket02->bDrawDebugLagMarkers = true;
		WeaponSocket02->CameraLagSpeed = 35.0f;
		WeaponSocket02->CameraRotationLagSpeed = 15.0f;
		WeaponSocket02->ComponentTags.Add(TEXT("Weapon02"));
	}
	
	bUseControllerRotationYaw = false;
}

void AAvatarCharacter::CameraPan(FVector2D Axis)
{
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}

void AAvatarCharacter::CameraZoom(float Delta, float Multiplier)
{
	if (IsValid(SpringArm))
	{
		float ArmLength = SpringArm->TargetArmLength;
		float NewLength = ArmLength + (Delta * Multiplier);
		SpringArm->TargetArmLength = FMath::Clamp(NewLength, CameraMinZoom, CameraMaxZoom);
	}
}


FGuid AAvatarCharacter::GetAssetInstanceId() const
{
	return AvatarInstance.AvatarId;
}

FPrimaryAssetId AAvatarCharacter::GetAssetId() const
{
	return CharacterAsset->GetPrimaryAssetId();
}

FPrimaryAssetType AAvatarCharacter::GetAssetType() const
{
	return CharacterAsset->GetPrimaryAssetType();
}

EDataSource AAvatarCharacter::GetSpawnSource() const
{
	return SourceType;
}


void AAvatarCharacter::InitializeCharacter()
{
	if (SourceType == EDataSource::Runtime)
	{
		UAvatarSubsystem* AvatarSubsystem = UAvatarSubsystem::Get(GetWorld());
		if (IsValid(AvatarSubsystem))
		{
			StorageManager = AvatarSubsystem->GetStorageManager();
			if (IsValid(StorageManager))
			{
				const FAvatarInstance* FoundInstance = StorageManager->GetInstance(CharacterData.AssetId);
				if (FoundInstance)
				{
					AvatarInstance = *FoundInstance;
					StorageManager->GetOnStorageUpdated().AddUObject(this, &AAvatarCharacter::RefreshCharacter);
				}
			}
		}
	}
	
	Super::InitializeCharacter();
}

void AAvatarCharacter::DeinitializeCharacter()
{
	if (IsValid(StorageManager))
	{
		StorageManager->GetOnStorageUpdated().RemoveAll(this);
	}
	StorageManager = nullptr;
	AvatarInstance.Reset();

	Super::DeinitializeCharacter();
}

void AAvatarCharacter::RefreshCharacter()
{
	if (!IsValid(StorageManager))
	{
		return;
	}

	const FAvatarInstance* FoundInstance = StorageManager->GetInstance(CharacterData.AssetId);
	if (!FoundInstance || AvatarInstance == *FoundInstance)
	{
		LOG_ERROR(LogAvatar, TEXT("Avatar data not found or not changed"));
		return;
	}

	AvatarInstance = *FoundInstance;

	SetCharacterLevel(FoundInstance->Ascension.Level);
	RefreshAttributes();
}


void AAvatarCharacter::AddRuntimeAttributes()
{
	if (SourceType == EDataSource::Runtime)
	{
		const UCharacterSettings* Settings = UCharacterSettings::Get();
		TMap<FGameplayTag, float>& Attributes = GetCharacterAttributes();

		Attributes.Add(Settings->DataHealthTag, AvatarInstance.Health);
	}
}

