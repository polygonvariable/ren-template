// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenAsset/Public/Inventory/Asset/Category/NonEnhanceableAsset.h"

// Generated Headers
#include "EnhanceAsset.generated.h"



/**
 * 
 */
UCLASS()
class RENASSET_API UEnhanceAsset : public UNonEnhanceableAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enhancement")
	int ExchangePoints = 500;

};

