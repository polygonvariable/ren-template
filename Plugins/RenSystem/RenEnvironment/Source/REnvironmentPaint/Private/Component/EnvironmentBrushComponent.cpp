// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/EnvironmentBrushComponent.h"

// Engine Headers
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Project Headers
#include "System/EnvironmentCanvasSubsystem.h"


UEnvironmentBrushComponent::UEnvironmentBrushComponent()
{
	bAutoActivate = false;
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
		if (IsValid(CharacterMovement) && CharacterMovement->MovementMode == MOVE_Falling)
		{
			return false;
		}
	}

	return bCanDraw && IsNearGround();
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

bool UEnvironmentBrushComponent::IsNearGround() const
{
	if (!bLineTrace)
	{
		return true;
	}

	const FVector Start = GetComponentToWorld().GetLocation() + FVector(0.0f, 0.0f, 20.0f);
	const FVector End = Start - FVector(0.0f, 0.0f, 60.0f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

#if WITH_EDITORONLY_DATA
	if (bDrawDebug)
	{
		DrawDebugDirectionalArrow(GetWorld(), Start, End, 10.0f, FColor::Yellow, false, -1.0f, 1, 2.5f);
	}
#endif

	FHitResult Hit;
	return GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams);
}

