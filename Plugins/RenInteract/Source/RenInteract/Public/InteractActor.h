// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenCore/Public/Actor/RegionActor.h"
#include "RenCore/Public/EventTimestamp/EventTimestampType.h"

#include "RenInteract/Public/InteractInterface.h"
#include "RenInteract/Public/InteractItem.h"

// Generated Headers
#include "InteractActor.generated.h"

// Forward Declarations
class UPrimitiveComponent;

class UInventoryAsset;
class IEventTimestampManagerInterface;



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
	virtual void StartInteract();

	UFUNCTION(BlueprintCallable)
	virtual void EndInteract();

	UFUNCTION(BlueprintCallable)
	virtual void UpdateInteract();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};



/**
 *
 */
UCLASS(Abstract)
class AInventoryPickupActor : public AInteractActor
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ContainerId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInventoryAsset> InventoryAsset = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int ItemQuantity = 1;

public:

	virtual void Interacted() override;

};



/**
 *
 */
UCLASS(Abstract)
class AInteractCooldownActor : public AInteractActor
{

	GENERATED_BODY()

public:

	AInteractCooldownActor();


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGuid CooldownGuid = FGuid::NewGuid();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTimespan CooldownDuration = FTimespan::FromMinutes(1);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bOnlyOnce = false;

protected:

	TWeakInterfacePtr<IEventTimestampManagerInterface> EventTimestampInterface;

	UFUNCTION(BlueprintImplementableEvent)
	void HandleEventCooldownStatus(EEventTimestampCooldownStatus Status);
	virtual void HandleEventCooldownStatus_Implementation(EEventTimestampCooldownStatus Status);

protected:

	virtual void Interacted() override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};

