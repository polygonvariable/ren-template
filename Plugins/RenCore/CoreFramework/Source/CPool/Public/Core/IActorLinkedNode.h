// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Generated Headers
#include "IActorLinkedNode.generated.h"


UINTERFACE(MinimalAPI)
class UActorLinkedNode : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class CPOOL_API IActorLinkedNode
{
	
	GENERATED_BODY()

public:

	virtual AActor* GetNextNode() const = 0;
	virtual void SetNextNode(AActor* Node) = 0;
};

