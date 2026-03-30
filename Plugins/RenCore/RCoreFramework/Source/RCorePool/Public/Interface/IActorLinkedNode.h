// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Generated Headers
#include "IActorLinkedNode.generated.h"

// Module Macros
#define REN_API RCOREPOOL_API



UINTERFACE(MinimalAPI)
class UActorLinkedNode : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class REN_API IActorLinkedNode
{
	
	GENERATED_BODY()

public:

	virtual AActor* GetNextNode() const = 0;
	virtual void SetNextNode(AActor* Node) = 0;
};


// Module Macros
#undef REN_API

