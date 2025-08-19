// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenInteract/Public/InteractActor.h"

// Generated Headers
#include "PickupActor.generated.h"

// Forward Declarations
class UPrimitiveComponent;
class USphereComponent;

class UInventoryAsset;
class UTimestampCooldownComponent;
class UInventorySubsystem;



/**
 *
 */
UCLASS(Abstract)
class APickupActor : public AInteractActor
{

	GENERATED_BODY()

public:

	APickupActor();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USphereComponent> SphereCollision;

protected:

	virtual UPrimitiveComponent* GetCollisionComponent_Implementation() const override;

};


/**
 *
 */
UCLASS(Abstract)
class ASpawnablePickupActor : public APickupActor
{

	GENERATED_BODY()

public:

	ASpawnablePickupActor();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UTimestampCooldownComponent> TimestampComponent;

protected:

	virtual void BeginPlay() override;

};


/**
 *
 */
UCLASS(Abstract)
class AInventoryPickupActor : public ASpawnablePickupActor
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ContainerId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bHideAfterPickup = false;

	virtual void Interacted() override;

protected:

	virtual UInventorySubsystem* GetInventorySubsystem() const;

};



/**
 *
 */
UCLASS()
class AInventoryItemActor : public AInventoryPickupActor
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int ItemQuantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInventoryAsset> InventoryAsset;

	virtual void Interacted() override;

};


/**
 *
 */
UCLASS()
class AInventoryChestActor : public AInventoryPickupActor
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<UInventoryAsset*, int> InventoryAssets;

	virtual void Interacted() override;

};


