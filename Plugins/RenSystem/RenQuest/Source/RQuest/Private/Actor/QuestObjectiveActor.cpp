// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/QuestObjectiveActor.h"

// Engine Headers
#include "Components/ShapeComponent.h"


AQuestObjectiveMarker::AQuestObjectiveMarker()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void AQuestObjectiveMarker::HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DoesCollidedWithPlayer(OtherActor) && !bPlayerInRegion)
	{
		OnInteractionCompleted.ExecuteIfBound(EFSMResult::Success);
		bPlayerInRegion = true;
	}
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
		OnInteractionCompleted.ExecuteIfBound(EFSMResult::Success);
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

