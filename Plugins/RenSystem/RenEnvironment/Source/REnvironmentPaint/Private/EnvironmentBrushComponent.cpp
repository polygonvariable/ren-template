// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EnvironmentBrushComponent.h"

// Engine Headers
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Project Headers
#include "System/EnvironmentCanvasSubsystem.h"


UEnvironmentBrushComponent::UEnvironmentBrushComponent()
{
	bAutoActivate = true;
	PrimaryComponentTick.bCanEverTick = false;
}


void UEnvironmentBrushComponent::RegisterBrush()
{
	UEnvironmentCanvasSubsystem* Subsystem = UEnvironmentCanvasSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->RegisterBrush(this);
	}
}

void UEnvironmentBrushComponent::UnregisterBrush()
{
	UEnvironmentCanvasSubsystem* Subsystem = UEnvironmentCanvasSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->UnregisterBrush(this);
	}
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


bool UEnvironmentBrushComponent::GetBrushDetails(FVector& Location, FVector2D& Size, float& Density)
{
	Location = GetComponentToWorld().GetLocation();
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

bool UEnvironmentBrushComponent::GetBrushDetails(FVector& Location, FVector& Velocity, FVector2D& Size, float& Density)
{
	Location = GetComponentToWorld().GetLocation();
	Density = BrushDensity;
	Velocity = GetComponentVelocity();
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


void UEnvironmentBrushComponent::Activate(bool bReset)
{
	Super::Activate(bReset);

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		CharacterMovement = Character->GetCharacterMovement();
		bIsCharacter = true;
	}

	RegisterBrush();
}

void UEnvironmentBrushComponent::Deactivate()
{
	UnregisterBrush();
	CharacterMovement = nullptr;

	Super::Deactivate();
}

void UEnvironmentBrushComponent::EndPlay(EEndPlayReason::Type Reason)
{
	UnregisterBrush();
	Super::EndPlay(Reason);
}

