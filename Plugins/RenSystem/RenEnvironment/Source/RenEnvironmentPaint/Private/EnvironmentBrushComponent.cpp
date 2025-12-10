// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EnvironmentBrushComponent.h"

// Engine Headers
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Project Headers



void UEnvironmentBrushComponent::Activate(bool bReset)
{
	Super::Activate(bReset);

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		CharacterMovement = Character->GetCharacterMovement();
		bIsCharacter = true;
	}
}

void UEnvironmentBrushComponent::Deactivate()
{
	CharacterMovement = nullptr;

	Super::Deactivate();
}


void UEnvironmentBrushComponent::SetCanDraw(bool bEnable)
{
	bCanDraw = bEnable;
}

void UEnvironmentBrushComponent::SetBrushDensity(float Density)
{
	BrushDensity = FMath::Clamp(Density, 0.0f, 1.0f);
}

void UEnvironmentBrushComponent::SetBrushSize(FVector2D Size)
{
	BrushSize = Size;
}


bool UEnvironmentBrushComponent::GetBrushDetails(FVector& Location, FVector& Velocity, FVector2D& Size, float& Density)
{
	const FTransform& Transform = GetComponentToWorld();
	Location = Transform.GetLocation();
	Velocity = GetComponentVelocity();
	Density = BrushDensity;
	Size = BrushSize;

	if (bIsCharacter)
	{
		if (!IsValid(CharacterMovement) || CharacterMovement->MovementMode != MOVE_Walking)
		{
			return false;
		}
	}

	return bCanDraw;
}

