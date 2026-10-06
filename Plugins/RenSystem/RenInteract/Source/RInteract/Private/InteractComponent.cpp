// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InteractComponent.h"

// Engine Headers
#include "Components/ShapeComponent.h"
#include "GameFramework/Pawn.h"

// Project Headers
#include "InteractSubsystem.h"
#include "GameplayContextInterface.h"


UInteractComponent::UInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	bAutoActivate = false;
	InteractId = FGuid::NewGuid();
}

void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	InteractSubsystem = UInteractSubsystem::Get(GetWorld());

	UPrimitiveComponent* CollisionComponent = GetCollisionComponent();
	if (IsValid(CollisionComponent))
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &UInteractComponent::HandlePlayerEntered);
		CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &UInteractComponent::HandlePlayerExited);
	}
}

void UInteractComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
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

void UInteractComponent::OnInteracted()
{
	AActor* Owner = GetOwner();
	if (bHideAfterInteract)
	{
		Owner->SetActorHiddenInGame(true);
		Owner->SetActorEnableCollision(false);
	}

	UActorComponent* ContextComponent = Owner->FindComponentByInterface(UGameplayContextInterface::StaticClass());
	if (IsValid(ContextComponent))
	{
		IGameplayContextInterface* ContextInterface = Cast<IGameplayContextInterface>(ContextComponent);
		if (ContextInterface)
		{
			ContextInterface->Execute(ContextTags, this);
		}
	}
}

UPrimitiveComponent* UInteractComponent::GetCollisionComponent() const
{
	return GetOwner()->FindComponentByClass<UShapeComponent>();
}

bool UInteractComponent::CollisionCondition(AActor* Actor) const
{
	APawn* Character = Cast<APawn>(Actor);
	if (!IsValid(Character) || !IsValid(InteractSubsystem))
	{
		return false;
	}
	return Character->IsPlayerControlled();
}

void UInteractComponent::HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CollisionCondition(OtherActor) && !_bIsInteracting)
	{
		InteractSubsystem->RegisterItem(InteractId, this, InteractItem);
		_bIsInteracting = true;
	}
}

void UInteractComponent::HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	if (CollisionCondition(OtherActor) && _bIsInteracting)
	{
		InteractSubsystem->UnregisterItem(InteractId);
		_bIsInteracting = false;
	}
}

