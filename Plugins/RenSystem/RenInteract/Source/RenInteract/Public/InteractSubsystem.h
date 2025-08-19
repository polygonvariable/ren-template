// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "RenInteract/Public/InteractInterface.h"

// Generated Headers
#include "InteractSubsystem.generated.h"

// Forward Declarations
struct FInteractItem;



/**
 *
 */
UCLASS()
class UInteractSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	FOnInteractStarted OnInteractStarted;

	FOnInteractEnded OnInteractEnded;


	UFUNCTION(BlueprintCallable)
	void EnableInteract();

	UFUNCTION(BlueprintCallable)
	void DisableInteract();

	void AddItem(AActor* Actor, const FInteractItem* Item);
	void RemoveItem(AActor* Actor);

protected:

	bool bInteractEnabled = true;

	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

};

