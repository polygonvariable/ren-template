// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "RenCore/Public/EventTimestamp/EventTimestampManagerInterface.h"
#include "RenCore/Public/EventTimestamp/EventTimestampType.h"

// Generated Headers
#include "EventTimestampSubsystem.generated.h"

// Forward Declarations
class IEventTimestampProviderInterface;



/**
 *
 */
UCLASS()
class UEventTimestampSubsystem : public UWorldSubsystem, public IEventTimestampManagerInterface
{

	GENERATED_BODY()

public:

	virtual bool AddHistory(FName Guid, bool bForceAdd) override;
	bool RemoveHistory(FName Guid);
	bool ContainsHistory(FName Guid) const;
	bool GetHistoryDate(FName Guid, FDateTime& Date) const;

	TEnumAsByte<EEventTimestampStatus> GetHistoryStatus(FName Guid) const;
	virtual TEnumAsByte<EEventTimestampCooldownStatus> GetHistoryCooldownStatus(FName Guid, bool bOnlyOnce, const FTimespan& CooldownTime) const override;

protected:

	TWeakInterfacePtr<IEventTimestampProviderInterface> EventTimestampInterface;

	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;

};

