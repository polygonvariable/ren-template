// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/Actor.h"

// Project Headers
#include "GameplayContextAction.h"
#include "RegionActor.h"
#include "StateMachine/FiniteStateMachineType.h"

// Generated Headers
#include "QuestObjectiveActor.generated.h"

// Forward Declarations
class UEventflowTask;


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


	DECLARE_DELEGATE(FOnInteractionCompleted);
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




/*
 *
 */
USTRUCT(DisplayName = "Quest Interact Handle")
struct FGameplayContext_QuestInteractionHandle : public FGameplayContextAction
{

	GENERATED_BODY()

public:

	// ~ FGameplayContextAction
	virtual bool Execute(UWorld* World, UObject* Owner, UObject* Instigator) override;
	// ~ End of FGameplayContextAction

};