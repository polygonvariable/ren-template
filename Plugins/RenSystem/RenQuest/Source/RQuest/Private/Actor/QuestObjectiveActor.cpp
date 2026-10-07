// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/QuestObjectiveActor.h"

#include "Components/ShapeComponent.h"

// Project Headers
#include "EventflowTask.h"
#include "GameplayContextInterface.h"








AQuestObjectiveMarker::AQuestObjectiveMarker()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void AQuestObjectiveMarker::HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DoesCollidedWithPlayer(OtherActor) && !bPlayerInRegion)
	{
		OnInteractionCompleted.ExecuteIfBound();
		bPlayerInRegion = true;
	}
}


bool FGameplayContext_QuestInteractionHandle::Execute(UWorld* World, UObject* Owner, UObject* Instigator)
{
	UActorComponent* Component = Cast<UActorComponent>(Owner);
	if (!::IsValid(Component))
	{
		return false;
	}

	AQuestInteractionMarker* InteractActor = Cast<AQuestInteractionMarker>(Component->GetOwner());
	if (!::IsValid(Component))
	{
		return false;
	}

	InteractActor->OnInteractionCompleted.ExecuteIfBound();
	return true;
}







UPrimitiveComponent* AObjectiveCascadeMarker::GetCollisionComponent_Implementation() const
{
	return FindComponentByClass<UShapeComponent>();
}

void AObjectiveCascadeMarker::InitialMarkerLocation()
{
	UPrimitiveComponent* Component = GetCollisionComponent();
	if (IsValid(Component) && Locations.IsValidIndex(Index))
	{
		Component->SetWorldLocation(Locations[Index]);
	}
}

void AObjectiveCascadeMarker::UpdateMarkerLocation()
{
	Index++;
	if (!Locations.IsValidIndex(Index))
	{
		OnInteractionCompleted.ExecuteIfBound();
		return;
	}

	UPrimitiveComponent* Component = GetCollisionComponent();
	if (IsValid(Component))
	{
		Component->SetWorldLocation(Locations[Index]);
	}
}

void AObjectiveCascadeMarker::HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DoesCollidedWithPlayer(OtherActor) && !bPlayerInRegion)
	{
		UpdateMarkerLocation();
		bPlayerInRegion = true;
	}
}

void AObjectiveCascadeMarker::HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	if (DoesCollidedWithPlayer(OtherActor) && bPlayerInRegion)
	{
		bPlayerInRegion = false;
	}
}

void AObjectiveCascadeMarker::BeginPlay()
{
	Super::BeginPlay();

	InitialMarkerLocation();
}

