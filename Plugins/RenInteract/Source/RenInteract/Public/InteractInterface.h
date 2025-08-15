// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "InteractInterface.generated.h"

// Forward Declarations
struct FInteractItem;

// Delegate Declarations
DECLARE_MULTICAST_DELEGATE(FOnInteracted);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInteractStarted, AActor*, const FInteractItem*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInteractEnded, AActor*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInteractUpdated, const FInteractItem*);



UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class IInteractInterface
{
	GENERATED_BODY()

public:

	virtual void Interacted() = 0;

	virtual FOnInteracted& GetOnInteracted() = 0;
	virtual FOnInteractUpdated& GetOnInteractUpdated() = 0;

};


