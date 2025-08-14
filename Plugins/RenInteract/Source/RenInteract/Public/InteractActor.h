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
class AInteractActor : public ARegionActor, public IInteractInterface
{

	GENERATED_BODY()
	
public:

	AInteractActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInteractItem InteractItem;

protected:

	FOnInteracted OnInteracted;
	FOnInteractUpdated OnInteractUpdated;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_Interacted();

public:

	virtual void Interacted() override;

	virtual FOnInteracted& GetOnInteracted() override;
	virtual FOnInteractUpdated& GetOnInteractUpdated() override;

protected:

	virtual void HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex) override;

	UFUNCTION(BlueprintCallable)
	virtual void StartInteract() override;

	UFUNCTION(BlueprintCallable)
	virtual void EndInteract() override;

	UFUNCTION(BlueprintCallable)
	virtual void UpdateInteract() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};

