// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"

// Project Headers

// Generated Headers
#include "RGameplayAbility.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS()
class URGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	FGameplayTagContainer GetDynamicTags(bool& bSuccess) const;

};

