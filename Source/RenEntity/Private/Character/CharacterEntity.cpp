// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Character/CharacterEntity.h"

// Engine Headers
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"
#include "WeaponTest.h"
#include "RenAbility/Public/Attributes/DamageAttributeSet.h"
#include "RenAbility/Public/Library/AbilitySystemLibrary.h"
#include "RenAbility/Public/Component/RAbilitySystemComponent.h"



ACharacterEntity::ACharacterEntity() : Super()
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
	}

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	if (IsValid(Camera))
	{
		Camera->SetupAttachment(SpringArm);
	}

	RAbilitySystemComponent = CreateDefaultSubobject<URAbilitySystemComponent>(TEXT("RAbilitySystemComponent"));

	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
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

	bUseControllerRotationYaw = false;
}

void ACharacterEntity::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	RAbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
}


UAbilitySystemComponent* ACharacterEntity::GetAbilitySystemComponent() const
{
	return RAbilitySystemComponent;
}

void ACharacterEntity::CameraPan(const FVector2D Axis)
{
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}

void ACharacterEntity::CameraZoom(const float Delta, const float Multiplier)
{
	if(!IsValid(SpringArm))
	{
		return;
	}

	float CurrentLength = SpringArm->TargetArmLength;
	SpringArm->TargetArmLength = FMath::Clamp(CurrentLength + (Delta * Multiplier), CameraMinZoom, CameraMaxZoom);
}


void ACharacterEntity::SimpleMove_Implementation(FVector direction)
{
	FRotator Rotation = GetControlRotation();
	FVector RightVector = UKismetMathLibrary::GetRightVector(FRotator(0.0f, Rotation.Yaw, Rotation.Roll));
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, Rotation.Yaw, 0.0f));

	AddMovementInput(RightVector, direction.X, false);
	AddMovementInput(ForwardVector, direction.Y, false);
}

void ACharacterEntity::DealDamage(TSubclassOf<UGameplayEffect> EffectClass, AActor* Target, AActor* EffectCauser)
{
	// If the target has lazy loading of ASC, try to call the function
	// to load the ASC and then apply the effect here


	UAbilitySystemComponent* TargetASC = Target->GetComponentByClass<UAbilitySystemComponent>();

	if (IsValid(RAbilitySystemComponent) && IsValid(TargetASC))
	{
		/*TArray<TWeakObjectPtr<AActor>> ContextActors;
		for (AActor* Actor : OwnedActors)
		{
			ContextActors.Add(TWeakObjectPtr<AActor>(Actor));
		}*/

		/*UAbilitySystemLibrary::ApplyGameplayEffectToTarget(Target, this, this, EffectClass, 1.0f, [&](FGameplayEffectContextHandle& EffectContext) {
			EffectContext.AddActors(ContextActors);
			EffectContext.AddOrigin(GetActorLocation());
		});*/

		TArray<TWeakObjectPtr<AActor>> TargetActors;
		TargetActors.Add(TWeakObjectPtr<AActor>(Target));

		FGameplayEffectContextHandle EffectContext = RAbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		EffectContext.AddActors(TargetActors);
		EffectContext.AddOrigin(GetActorLocation());
		EffectContext.AddInstigator(this, EffectCauser);

		if (!EffectContext.IsValid())
		{
			PRINT_ERROR(LogTemp, 1.0f, TEXT("Failed to create effect context"));
			return;
		}

		FGameplayEffectSpecHandle SpecHandle = RAbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1.0f, EffectContext);
		if (!SpecHandle.IsValid())
		{
			PRINT_ERROR(LogTemp, 1.0f, TEXT("Failed to create effect spec handle"));
			return;
		}

		RAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
	}
}

void ACharacterEntity::SpawnWeapon()
{
	if (IsValid(Weapon) || !GetWorld())
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Weapon is already spawned or World is not valid"));
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = this;

	Weapon = GetWorld()->SpawnActor<AWeaponTest>(WeaponClass, GetActorLocation(), GetActorRotation(), SpawnParams);
	if (IsValid(Weapon))
	{
		FGameplayAttribute PhysicalDamageAttribute = UDamageAttributeSet::GetPhysicalDamageAttribute();
		OwnedActors.Add(Weapon);
		RAbilitySystemComponent->AddAggregatedActor(PhysicalDamageAttribute, Weapon);
	}

	PRINT_WARNING(LogTemp, 1.0f, TEXT("Weapon spawned"));


}


//void ACharacterEntity::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
//	{
//		EMovementMode Mode = MovementComponent->MovementMode;
//		switch (Mode)
//		{
//			case EMovementMode::MOVE_Walking:
//				if (GetVelocity().IsNearlyZero(50.0f))
//				{
//					CurrentSubMovement = ESubMovement::Idle;
//				}
//				else if (GetVelocity().Size2D() > 50.0f && GetVelocity().Size2D() < 500.0f || !bCanSprint)
//				{
//					CurrentSubMovement = ESubMovement::Walk;
//				}
//				else if (GetVelocity().Size2D() > 500.0f && bCanSprint)
//				{
//					CurrentSubMovement = ESubMovement::Sprint;
//				}
//				break;
//			case EMovementMode::MOVE_Falling:
//				CurrentSubMovement = ESubMovement::Fall;
//				break;
//		}
//	}
//}
// On_Jump_Pressed -> Give jump ability
// On_Jump_Released -> Remove jump ability

