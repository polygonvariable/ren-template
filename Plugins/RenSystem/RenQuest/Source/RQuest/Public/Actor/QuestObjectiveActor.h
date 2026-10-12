// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/Actor.h"

// Project Headers
#include "Core/IObjectiveFeedback.h"
#include "RegionActor.h"

// Generated Headers
#include "QuestObjectiveActor.generated.h"


/*
 *
 */
UCLASS(Abstract)
class AQuestObjectiveActor : public ARegionActor
{
	GENERATED_BODY()
};


/*
 *
 */
UCLASS(Abstract)
class AQuestObjectiveMarker : public ARegionActor
{

	GENERATED_BODY()

public:

	AQuestObjectiveMarker();

	FOnInteractionCompleted OnInteractionCompleted;

protected:

	// ~ ARegionActor
	virtual void HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	// ~ End of ARegionActor

};


/*
 *
 */
UCLASS(Abstract)
class AObjectiveCascadeMarker : public AQuestObjectiveMarker
{

	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	int Index = 0;

	UPROPERTY(EditAnywhere)
	TArray<FVector> Locations;

protected:

	virtual UPrimitiveComponent* GetCollisionComponent_Implementation() const override;

	void InitialMarkerLocation();
	void UpdateMarkerLocation();

	// ~ ARegionActor
	virtual void HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex) override;
	// ~ End of ARegionActor

	// ~ AActor
	virtual void BeginPlay() override;
	// ~ End of AActor

};


/*
 *
 */
UCLASS(Abstract)
class AQuestInteractionMarker : public AQuestObjectiveMarker
{
	GENERATED_BODY()
};

