// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/CharacterBase.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Project Headers
#include "Asset/CharacterAsset.h"
#include "Library/AscensionLibrary.h"
#include "Settings/CharacterSettings.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


ACharacterBase::ACharacterBase() : Super()
{
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	if (IsValid(Capsule))
	{
		Capsule->SetCapsuleHalfHeight(90.0f);
		Capsule->SetCapsuleRadius(35.0f);
	}

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (IsValid(MovementComponent))
	{
		MovementComponent->GravityScale = 1.75f;
		MovementComponent->MaxAcceleration = 1500.0f;
		MovementComponent->bUseSeparateBrakingFriction = true;

		MovementComponent->MaxWalkSpeed = 500.0f;
		MovementComponent->MinAnalogWalkSpeed = 20.0f;
		MovementComponent->BrakingDecelerationWalking = 2000.0f;

		MovementComponent->BrakingDecelerationFalling = 1500.0f;
		MovementComponent->AirControl = 0.35f;

		MovementComponent->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
		MovementComponent->bOrientRotationToMovement = true;
	}
	
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	PrimaryActorTick.bStartWithTickEnabled = false;
}


bool ACharacterBase::IsAlive() const
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC))
	{
		return false;
	}
	return ASC->HasMatchingGameplayTag(UCharacterSettings::Get()->StateAliveTag);
}

void ACharacterBase::InitializeCharacter()
{
	InitializeAttributes();
	InitializeTags();
}

void ACharacterBase::DeinitializeCharacter()
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (IsValid(MovementComponent))
	{
		MovementComponent->SetMovementMode(MOVE_None);
	}

	CharacterAsset = nullptr;
}

void ACharacterBase::RefreshCharacter()
{
	RefreshAttributes();
}





void ACharacterBase::InitializeAttributes()
{
	_CharacterAttributes = CharacterData.Attributes;

	AddDefaultAttributes();
	AddRuntimeAttributes();
	ApplyAttributes();
}

void ACharacterBase::RefreshAttributes()
{
	AddRuntimeAttributes();
	ApplyAttributes();
}

void ACharacterBase::AddDefaultAttributes()
{
	if (!IsValid(CharacterAsset))
	{
		return;
	}

	const UCharacterSettings* Settings = UCharacterSettings::Get();

	_CharacterAttributes.Add(Settings->DataMaxHealthTag, CharacterAsset->Health);

	_CharacterAttributes.Add(Settings->DataPhysicalDamageTag, CharacterAsset->PhysicalDamage);
	_CharacterAttributes.Add(Settings->DataPhysicalDefenseTag, CharacterAsset->PhysicalDefense);

	_CharacterAttributes.Add(Settings->DataElementalDamageTag, CharacterAsset->ElementalDamage);
	_CharacterAttributes.Add(Settings->DataElementalDefenseTag, CharacterAsset->ElementalDefense);
}

void ACharacterBase::AddRuntimeAttributes()
{
}

void ACharacterBase::ApplyAttributes()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC))
	{
		return;
	}

	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.AddSourceObject(this);

	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(InitialAttributeEffectClass, GetCharacterLevel(), Context);
	if (Spec.IsValid())
	{
		Spec.Data->SetByCallerTagMagnitudes = _CharacterAttributes;
		ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
}

int ACharacterBase::GetCharacterLevel() const
{
	const FGameplayTag& LevelTag = UCharacterSettings::Get()->DataLevelTag;
	const float* Level = _CharacterAttributes.Find(LevelTag);
	if (!Level || !IsValid(CharacterAsset))
	{
		return _CharacterLevel;
	}

	return FMath::Clamp(*Level, 1.0f, CharacterAsset->GetMaxLevel());
}

void ACharacterBase::SetCharacterLevel(int Level)
{
	_CharacterLevel = Level;
}

TMap<FGameplayTag, float>& ACharacterBase::GetCharacterAttributes()
{
	return _CharacterAttributes;
}





void ACharacterBase::InitializeTags()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC))
	{
		return;
	}

	const UCharacterSettings* Settings = UCharacterSettings::Get();

	FGameplayTag HealthTag;
	int Health = _CharacterAttributes.FindRef(Settings->DataHealthTag);
	if (Health > 0)
	{
		HealthTag = Settings->StateAliveTag;
	}
	else
	{
		HealthTag = Settings->StateDeadTag;
	}

	ASC->AddLooseGameplayTag(HealthTag);
}





void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void ACharacterBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	const UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (IsValid(ASC))
	{
		ASC->GetOwnedGameplayTags(TagContainer);
	}
}

bool ACharacterBase::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	const UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC))
	{
		return false;
	}
	return ASC->HasMatchingGameplayTag(TagToCheck);
}

bool ACharacterBase::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	const UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC))
	{
		return false;
	}
	return ASC->HasAllMatchingGameplayTags(TagContainer);
}

bool ACharacterBase::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	const UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC))
	{
		return false;
	}
	return ASC->HasAnyMatchingGameplayTags(TagContainer);
}

void ACharacterBase::GetSpawnData(const FGameplayTag& InTag, FInstancedStruct& OutValue) const
{
	const FInstancedStruct* Value = CharacterData.Metadata.Find(InTag);
	if (Value)
	{
		OutValue = *Value;
	}
}


void ACharacterBase::DirectionalMove_Implementation(const FVector& Direction)
{
	FRotator Rotation = GetControlRotation();
	FVector RightVector = UKismetMathLibrary::GetRightVector(FRotator(0.0f, Rotation.Yaw, Rotation.Roll));
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, Rotation.Yaw, 0.0f));

	AddMovementInput(RightVector, Direction.X, false);
	AddMovementInput(ForwardVector, Direction.Y, false);
}

bool ACharacterBase::IsMoving(float Threshold) const
{
	FVector Velocity = GetVelocity();
	float Speed = Velocity.Size2D();

	return Speed > Threshold;
}

