// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "SeasonProviderInterface.generated.h"

// Forward Declarations
class UPrimaryDataAsset;

// Delegates Declarations
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSeasonChanged, UPrimaryDataAsset* /* Season Asset */);



UINTERFACE(MinimalAPI)
class USeasonProviderInterface : public UInterface
{
	GENERATED_BODY()
};

class RENENVIRONMENT_API ISeasonProviderInterface
{
	GENERATED_BODY()

public:

	virtual FOnSeasonChanged& GetOnSeasonChanged() = 0;
	virtual UPrimaryDataAsset* GetSeasonAsset() = 0;
	virtual const FName GetSeasonName() const = 0;
	
};

