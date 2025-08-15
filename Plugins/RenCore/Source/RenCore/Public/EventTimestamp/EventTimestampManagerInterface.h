// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers
#include "RenCore/Public/EventTimestamp/EventTimestampType.h"

// Generated Headers
#include "EventTimestampManagerInterface.generated.h"

// Forward Declarations



UINTERFACE(MinimalAPI)
class UEventTimestampManagerInterface : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class IEventTimestampManagerInterface
{

	GENERATED_BODY()

public:

	RENCORE_API virtual bool AddHistory(FName Guid, bool bForceAdd) = 0;
	RENCORE_API virtual TEnumAsByte<EEventTimestampCooldownStatus> GetHistoryCooldownStatus(FName Guid, bool bOnlyOnce, const FTimespan& CooldownTime) const = 0;

};

