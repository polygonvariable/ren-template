// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/Actor.h"

// Generated Headers
#include "InteractActor.generated.h"

// Forward Declarations
class USphereComponent;
class UInteractComponent;


/**
 *
 */
UCLASS(Abstract)
class AInteractActor : public AActor
{

	GENERATED_BODY()
	
public:

	AInteractActor();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UInteractComponent> InteractComponent;

};

