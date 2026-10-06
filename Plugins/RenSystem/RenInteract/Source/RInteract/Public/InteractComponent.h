// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "InteractItem.h"

// Generated Headers
#include "InteractComponent.generated.h"

// Forward Declarations
class FObjectPreSaveContext;
class UPrimitiveComponent;
class UInteractSubsystem;


/**
 *
 */
UCLASS(MinimalAPI, meta = (BlueprintSpawnableComponent))
class UInteractComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	UInteractComponent();

	UPROPERTY(EditAnywhere)
	FGuid InteractId;

	UPROPERTY(EditAnywhere)
	FInteractItem InteractItem;


	void OnInteracted();
	
	// ~ UActorComponent
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~ End of UActorComponent

protected:

	UPROPERTY()
	TObjectPtr<UInteractSubsystem> InteractSubsystem = nullptr;


	UPrimitiveComponent* GetCollisionComponent() const;
	bool CollisionCondition(AActor* Actor) const;

	UFUNCTION()
	virtual void HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex);

private:

	bool _bIsInteracting = false;

};

