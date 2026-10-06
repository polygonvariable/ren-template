// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InteractActor.h"

// Engine Headers
#include "Components/SphereComponent.h"

// Project Headers
#include "InteractComponent.h"


AInteractActor::AInteractActor()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	if (IsValid(SceneComponent))
	{
		SetRootComponent(SceneComponent);

		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
		if (IsValid(CollisionComponent))
		{
			CollisionComponent->SetupAttachment(SceneComponent);
			CollisionComponent->SetSphereRadius(150.0);
			CollisionComponent->SetLineThickness(5.0f);
#if WITH_EDITOR
			CollisionComponent->bHiddenInGame = false;
#endif
		}

		InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("Interact"));
	}

	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SetCanBeDamaged(false);
}

