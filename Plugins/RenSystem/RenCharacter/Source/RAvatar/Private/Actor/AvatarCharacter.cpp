// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/AvatarCharacter.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Project Headers
#include "Asset/CharacterAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Settings/AvatarSettings.h"
#include "Settings/CharacterSettings.h"
#include "Storage/AvatarStorage.h"
#include "Subsystem/AvatarSubsystem.h"


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
		float NewLength = SpringArm->TargetArmLength + (Delta * Multiplier);
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

void AAvatarCharacter::InitializeCharacter()
{
	if (CharacterData.SourceType == EAssetQuerySource::Asset)
	{
		Super::InitializeCharacter();
		return;
	}
	
	UAvatarSubsystem* AvatarSubsystem = UAvatarSubsystem::Get(GetWorld());
	if (!IsValid(AvatarSubsystem))
	{
		LOG_ERROR(LogAvatar, TEXT("AvatarSubsystem is invalid"));
		return;
	}

	AvatarStorage = AvatarSubsystem->GetAvatarCollection();
	if (!IsValid(AvatarStorage))
	{
		LOG_ERROR(LogAvatar, TEXT("AvatarStorage is invalid"));
		return;
	}
	
	const FAvatarInstance* FoundInstance = AvatarStorage->GetInstance(CharacterData.AssetId);
	if (!FoundInstance)
	{
		LOG_ERROR(LogAvatar, TEXT("Avatar data not found"));
		return;
	}

	AvatarInstance = *FoundInstance;
	AvatarStorage->OnStorageUpdated.AddUObject(this, &AAvatarCharacter::RefreshCharacter);

	Super::InitializeCharacter();
}

void AAvatarCharacter::DeinitializeCharacter()
{
	if (IsValid(AvatarStorage))
	{
		AvatarStorage->OnStorageUpdated.RemoveAll(this);
	}
	AvatarStorage = nullptr;
	AvatarInstance.Reset();

	Super::DeinitializeCharacter();
}

void AAvatarCharacter::RefreshCharacter()
{
	if (!IsValid(AvatarStorage))
	{
		return;
	}

	const FAvatarInstance* FoundInstance = AvatarStorage->GetInstance(CharacterData.AssetId);
	if (!FoundInstance || AvatarInstance == *FoundInstance)
	{
		LOG_ERROR(LogAvatar, TEXT("Avatar data not found or not changed"));
		return;
	}

	AvatarInstance = *FoundInstance;

	RefreshAttributes();
}


void AAvatarCharacter::AddRuntimeAttributes()
{
	if (CharacterData.SourceType == EAssetQuerySource::Instance)
	{
		const UCharacterSettings* Settings = UCharacterSettings::Get();
		TMap<FGameplayTag, float>& Attributes = CharacterData.Attributes;

		Attributes.Add(Settings->AttributeHealthTag, AvatarInstance.Health);
	}
}

int AAvatarCharacter::GetCharacterLevel() const
{
	if (CharacterData.SourceType == EAssetQuerySource::Instance)
	{
		return AvatarInstance.Ascension.Level;
	}
	return Super::GetCharacterLevel();
}

