// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenTimestamp/Public/TimestampComponent.h"
#include "RenTimestamp/Public/TimestampStatus.h"

// Generated Headers
#include "TimestampCooldownComponent.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS(ClassGroup = (Custom), MinimalAPI)
class UTimestampCooldownComponent : public UTimestampComponent
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FTimespan CooldownTime = FTimespan::FromMinutes(1);

	UPROPERTY(EditAnywhere)
	bool bOnlyOnce = true;

	RENTIMESTAMP_API TEnumAsByte<ETimestampCooldownStatus> GetCooldownStatus() const;

};

