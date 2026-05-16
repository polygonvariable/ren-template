// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/InteractActor.h"

// Engine Headers
#include "GameFramework/Pawn.h"
#include "UObject/ObjectSaveContext.h"

// Project Headers
#include "Subsystem/InteractSubsystem.h"


AInteractActor::AInteractActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AInteractActor::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);

#if WITH_EDITOR
	InteractId = FGuid::NewGuid();
#endif
}

void AInteractActor::OnInteracted()
{
}

void AInteractActor::UpdateInteract()
{
	if (_bIsInteracting && IsValid(InteractSubsystem))
	{
		InteractSubsystem->RegisterItem(InteractId, this, InteractItem);
	}
}

UPrimitiveComponent* AInteractActor::GetCollisionComponent_Implementation() const
{
	return FindComponentByClass<UPrimitiveComponent>();
}

bool AInteractActor::CollisionCondition_Implementation(AActor* Actor) const
{
	APawn* Character = Cast<APawn>(Actor);
	if (!IsValid(Character))
	{
		return false;
	}
	return Character->IsPlayerControlled();
}

void AInteractActor::HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CollisionCondition(OtherActor) && IsValid(InteractSubsystem) && !_bIsInteracting)
	{
		InteractSubsystem->RegisterItem(InteractId, this, InteractItem);
		_bIsInteracting = true;
	}
}

void AInteractActor::HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	if (CollisionCondition(OtherActor) && IsValid(InteractSubsystem) && _bIsInteracting)
	{
		InteractSubsystem->UnregisterItem(InteractId);
		_bIsInteracting = false;
	}
}

void AInteractActor::BeginPlay()
{
	InteractSubsystem = UInteractSubsystem::Get(GetWorld());

	UPrimitiveComponent* CollisionComponent = GetCollisionComponent();
	if (IsValid(CollisionComponent))
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AInteractActor::HandlePlayerEntered);
		CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AInteractActor::HandlePlayerExited);
	}

	Super::BeginPlay();
}

void AInteractActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UPrimitiveComponent* CollisionComponent = GetCollisionComponent();
	if (IsValid(CollisionComponent))
	{
		CollisionComponent->OnComponentBeginOverlap.Clear();
		CollisionComponent->OnComponentEndOverlap.Clear();
	}

	if (IsValid(InteractSubsystem))
	{
		InteractSubsystem->UnregisterItem(InteractId);
	}
	InteractSubsystem = nullptr;

	Super::EndPlay(EndPlayReason);
}

