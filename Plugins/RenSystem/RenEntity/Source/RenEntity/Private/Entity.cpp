// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Entity.h"

// Engine Headers
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"



AEntity::AEntity() : Super()
{
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	if (IsValid(Capsule))
	{
		Capsule->SetCapsuleHalfHeight(90.0f);
		Capsule->SetCapsuleRadius(35.0f);
	}

	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AEntity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

