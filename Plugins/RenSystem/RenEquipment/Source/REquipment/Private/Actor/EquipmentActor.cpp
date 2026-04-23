// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/EquipmentActor.h"

AEquipmentActor::AEquipmentActor(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
	SetCanBeDamaged(false);
}


void AEquipmentActor::BeginPlay()
{
	Super::BeginPlay();
}

void AEquipmentActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AEquipmentActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


AActor* AEquipmentActor::GetNextNode() const
{
	return _NextNode;
}

void AEquipmentActor::SetNextNode(AActor* Node)
{
	_NextNode = Node;
}

