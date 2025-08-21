// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenCore/Public/Actor/RegionActor.h"

#include "RenInteract/Public/InteractInterface.h"
#include "RenInteract/Public/InteractItem.h"

// Generated Headers
#include "InteractActor.generated.h"

// Forward Declarations
class UPrimitiveComponent;



/**
 *
 */
UCLASS(Abstract)
class RENINTERACT_API AInteractActor : public ARegionActor, public IInteractInterface
{

	GENERATED_BODY()
	
public:

	AInteractActor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ShowOnlyInnerProperties))
	FInteractItem InteractItem;

protected:

	FOnInteracted OnInteracted;
	FOnInteractUpdated OnInteractUpdated;


	UFUNCTION(BlueprintImplementableEvent)
	void BP_Interacted();

	void StartInteract();
	void EndInteract();
	void UpdateInteract();

public:

	virtual void Interacted() override;

	virtual FOnInteracted& GetOnInteracted() override;
	virtual FOnInteractUpdated& GetOnInteractUpdated() override;

protected:

	virtual void HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};

