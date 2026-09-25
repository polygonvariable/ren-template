// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/CharacterBase.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "CharacterTrajectoryComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayEffect.h"
#include "Kismet/KismetMathLibrary.h"

// Project Headers
#include "Core/AscensionLibrary.h"
#include "Core/CharacterSettings.h"
#include "Data/AscensionFragment.h"
#include "Data/CharacterAsset.h"
#include "Data/CharacterTemplateAsset.h"
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

	USkeletalMeshComponent* SkeletonMesh = GetMesh();
	if (IsValid(SkeletonMesh))
	{
		SkeletonMesh->bAllowClothActors = false;
		SkeletonMesh->ComponentTags.Add(TEXT("Mesh"));
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
	
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("Ability System"));
	CharacterTrajectoryComponent = CreateDefaultSubobject<UCharacterTrajectoryComponent>(TEXT("Character Trajectory"));

	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}





void ACharacterBase::InitializeCharacter()
{
	SetSkeletonMesh();
	InitializeAttributes();
	InitializeComponents();
}

void ACharacterBase::DeinitializeCharacter()
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (IsValid(MovementComponent))
	{
		MovementComponent->SetMovementMode(MOVE_None);
	}
	
	CharacterAsset = nullptr;
	CharacterData.Reset();
	
	CharacterLevel = 1;
	CharacterAttributes.Empty();
	RuntimeComponents.Empty();
}

void ACharacterBase::RefreshCharacter()
{
	RefreshAttributes();
}





void ACharacterBase::SetSkeletonMesh()
{
	USkeletalMeshComponent* SkeletonMesh = GetMesh();
	check(SkeletonMesh);

	SkeletonMesh->SetSkeletalMesh(CharacterAsset->SkeletonMesh.Get());
	UAnimBlueprint* AnimBlueprint = CharacterAsset->AnimBlueprint.Get();
	if (IsValid(AnimBlueprint))
	{
		SkeletonMesh->SetAnimInstanceClass(AnimBlueprint->GeneratedClass);
	}
}





bool ACharacterBase::IsAlive() const
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC))
	{
		return false;
	}
	const UCharacterSettings* Settings = UCharacterSettings::Get();
	return !ASC->HasMatchingGameplayTag(Settings->StateDeadTag);
}


void ACharacterBase::CallOnCharacterDied()
{
	OnCharacterDied.Broadcast();
}

void ACharacterBase::CallOnCharacterRevived()
{
	OnCharacterRevived.Broadcast();
}








void ACharacterBase::InitializeComponents()
{
	TArray<FComponentDefinition>& Components = CharacterData.Components;
	if (Components.Num() > 0)
	{
		RegisterComponents(Components);
	}
	else
	{
		UCharacterTemplateAsset* TemplateAsset = CharacterAsset->CharacterTemplate.Get();
		if (IsValid(TemplateAsset))
		{
			RegisterComponents(TemplateAsset->Components);
		}
	}
}

void ACharacterBase::RegisterComponents(TArray<FComponentDefinition>& Components)
{
	RuntimeComponents.Reserve(Components.Num());

	for (FComponentDefinition& Definition : Components)
	{
		if (!Definition.IsValid())
		{
			continue;
		}

		FComponentTemplateData* InstanceData = Definition.Data.GetMutablePtr<FComponentTemplateData>();
		if (!InstanceData)
		{
			continue;
		}

		UActorComponent* NewComponent = NewObject<UActorComponent>(this, Definition.Component);
		if (IsValid(NewComponent))
		{
			NewComponent->RegisterComponent();

			InstanceData->AttachToParent(NewComponent, this);
			InstanceData->ApplyToInstance(NewComponent);

			AddInstanceComponent(NewComponent);

			RuntimeComponents.Add(NewComponent);
		}
	}
}

void ACharacterBase::ActivateComponents()
{
	for (UActorComponent* Component : RuntimeComponents)
	{
		if (IsValid(Component))
		{
			Component->Activate();
		}
	}
	RuntimeComponents.Empty();
}




void ACharacterBase::InitializeAttributes()
{
	CharacterAttributes = CharacterData.Attributes;

	AddDefaultAttributes();
	AddRuntimeAttributes();
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

	CharacterAttributes.Add(Settings->DataMaxHealthTag, CharacterAsset->Health);

	CharacterAttributes.Add(Settings->DataPhysicalDamageTag, CharacterAsset->PhysicalDamage);
	CharacterAttributes.Add(Settings->DataPhysicalDefenseTag, CharacterAsset->PhysicalDefense);

	CharacterAttributes.Add(Settings->DataElementalDamageTag, CharacterAsset->ElementalDamage);
	CharacterAttributes.Add(Settings->DataElementalDefenseTag, CharacterAsset->ElementalDefense);
}

void ACharacterBase::AddRuntimeAttributes()
{
}

void ACharacterBase::ApplyAttributes()
{
	if (!IsValid(CharacterAsset))
	{
		return;
	}

	const UCharacterTemplateAsset* TemplateAsset = CharacterAsset->CharacterTemplate.Get();
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(TemplateAsset) || !IsValid(TemplateAsset->InitialAttributeEffect) || !IsValid(ASC))
	{
		return;
	}

	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.AddSourceObject(this);

	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(TemplateAsset->InitialAttributeEffect, GetCharacterLevel(), Context);
	if (Spec.IsValid())
	{
		Spec.Data->SetByCallerTagMagnitudes = CharacterAttributes;
		ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
}







int ACharacterBase::GetCharacterLevel() const
{
	const UCharacterSettings* Settings = UCharacterSettings::Get();
	const float* Level = CharacterAttributes.Find(Settings->DataLevelTag);
	if (!Level || !IsValid(CharacterAsset))
	{
		return CharacterLevel;
	}

	const UAscensionFragment* Fragment = CharacterAsset->FindFragmentByClass<UAscensionFragment>();
	if (!IsValid(Fragment))
	{
		return CharacterLevel;
	}

	return FMath::Clamp(static_cast<int>(*Level), 1, Fragment->GetMaxLevel());
}

void ACharacterBase::SetCharacterLevel(int Level)
{
	const UAscensionFragment* Fragment = CharacterAsset->FindFragmentByClass<UAscensionFragment>();
	if (IsValid(Fragment))
	{
		CharacterLevel = FMath::Clamp(Level, 1, Fragment->GetMaxLevel());
	}
}

TMap<FGameplayTag, float>& ACharacterBase::GetCharacterAttributes()
{
	return CharacterAttributes;
}









UCharacterTrajectoryComponent* ACharacterBase::GetTrajectoryComponent() const
{
	return CharacterTrajectoryComponent;
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




void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check(ASC);

	ASC->InitAbilityActorInfo(this, this);
	ApplyAttributes();
	ActivateComponents();
}

void ACharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}


void ACharacterBase::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	const UCharacterSettings* Settings = UCharacterSettings::Get();
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	check(MovementComponent);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	ASC->RemoveLooseGameplayTag(Settings->ConvertMovementModeToTag(PrevMovementMode));
	ASC->AddLooseGameplayTag(Settings->ConvertMovementModeToTag(MovementComponent->MovementMode));
}




void ACharacterBase::DirectionalMove(const FVector& Direction)
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

