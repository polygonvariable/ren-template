// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"

// Generated Headers
#include "TagLibrary.generated.h"



/**
 * 
 * 
 */
UCLASS()
class UTagLibrary : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FGameplayTagContainer ConvertMovementModeToTag(EMovementMode MovementMode);

};

