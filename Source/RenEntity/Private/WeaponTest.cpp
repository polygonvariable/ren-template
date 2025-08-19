// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WeaponTest.h"
#include "Kismet/GameplayStatics.h"

// Engine Headers
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"

// Project Headers
#include "RenCoreLibrary/Public/LogMacro.h"


AWeaponTest::AWeaponTest()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}


void AWeaponTest::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(AbilitySystemComponent))
	{
		if (!GetOwner())
		{
			PRINT_ERROR(LogTemp, 2.0f, TEXT("Owner is not valid"));
			return;
		}
		
		AbilitySystemComponent->InitAbilityActorInfo(GetOwner(), this);

		AActor* NewOwner = AbilitySystemComponent->GetOwnerActor();
		AActor* NewAvatar = AbilitySystemComponent->GetAvatarActor();

		PRINT_WARNING(LogTemp, 2.0f, TEXT("NewOwner: %s, NewAvatar: %s"), *NewOwner->GetName(), *NewAvatar->GetName());
	}
	else
	{
		PRINT_ERROR(LogTemp, 2.0f, TEXT("AbilitySystemComponent is not valid"));
	}
}

void AWeaponTest::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

