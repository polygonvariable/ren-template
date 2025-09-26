// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "ExchangeInterface.generated.h"

// Forward Declarations
struct FInstancedStruct;



UINTERFACE(MinimalAPI)
class UExchangeInterface : public UInterface
{
	
	GENERATED_BODY()

};

/**
 * 
 */
class RCOREEXCHANGE_API IExchangeInterface
{

	GENERATED_BODY()

public:

	virtual float GetExchangedNumber(FInstancedStruct& Context) const = 0;
	virtual FName GetExchangedText(FInstancedStruct& Context) const = 0;

};



UINTERFACE(MinimalAPI)
class UExchangeManagerInterface : public UInterface
{
	
	GENERATED_BODY()

};

/**
 * 
 */
class RCOREEXCHANGE_API IExchangeManagerInterface
{

	GENERATED_BODY()

public:

	virtual void PerformExchange() = 0;

};

