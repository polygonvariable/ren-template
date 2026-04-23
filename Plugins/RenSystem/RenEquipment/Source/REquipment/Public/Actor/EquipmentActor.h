// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/Actor.h"

// Project Headers
#include "Interface/IActorLinkedNode.h"

// Generated Headers
#include "EquipmentActor.generated.h"

// Forward Declarations


/**
 * 
 */
UCLASS(Abstract)
class AEquipmentActor : public AActor, public IActorLinkedNode
{

	GENERATED_BODY()

public:

	AEquipmentActor(const FObjectInitializer& ObjectInitializer);

	// ~ IActorLinkedNode
	virtual AActor* GetNextNode() const override final;
	virtual void SetNextNode(AActor* Node) override final;
	// ~ End of IActorLinkedNode

	// ~ AActor
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~ End of AActor

protected:

	// ~ UAttributeSet
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ~ End of UAttributeSet

private:

	UPROPERTY()
	AActor* _NextNode = nullptr;

};

