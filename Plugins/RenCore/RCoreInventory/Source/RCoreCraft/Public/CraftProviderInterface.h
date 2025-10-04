// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "CraftProviderInterface.generated.h"

// Forward Declarations
struct FExchangeRule;



UINTERFACE(MinimalAPI)
class UCraftProviderInterface : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class RCORECRAFT_API ICraftProviderInterface
{

	GENERATED_BODY()

public:

	virtual const FExchangeRule& GetCraftingRule() const = 0;

};

