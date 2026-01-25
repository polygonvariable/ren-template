// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

// Generated Headers
#include "GameplayNotifyPacket.generated.h"

// Forward Declarations



/*
 * 
 * 
 * 
 */
USTRUCT(BlueprintType)
struct FGameplayNotifyPacket
{

	GENERATED_BODY()

public:

	FGameplayNotifyPacket() {};
	FGameplayNotifyPacket(float InMagnitude) : Magnitude(InMagnitude) {};
	FGameplayNotifyPacket(float InMagnitude, const FGameplayTagContainer& InTags) : Magnitude(InMagnitude), Tags(InTags) {};
	

	UPROPERTY(BlueprintReadOnly)
	float Magnitude = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTagContainer Tags;

};

