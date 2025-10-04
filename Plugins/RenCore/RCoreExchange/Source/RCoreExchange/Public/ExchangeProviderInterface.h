// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "ExchangeProviderInterface.generated.h"

// Forward Declarations
struct FInstancedStruct;



UINTERFACE(MinimalAPI)
class UExchangeProviderInterface : public UInterface
{
	
	GENERATED_BODY()

};

/**
 * 
 */
class RCOREEXCHANGE_API IExchangeProviderInterface
{

	GENERATED_BODY()

public:

	virtual float GetExchangedNumber(FInstancedStruct& Context) const = 0;
	virtual FName GetExchangedText(FInstancedStruct& Context) const = 0;

};

