// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "RenTimestamp/Public/TimestampStatus.h"

// Generated Headers
#include "TimestampSubsystem.generated.h"

// Forward Declarations
class ITimestampProviderInterface;



/**
 *
 */
UCLASS()
class UTimestampSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	bool AddTimestamp(FName TimestampId, bool bForceAdd);
	bool RemoveTimestamp(FName TimestampId);
	bool ContainsTimestamp(FName TimestampId) const;

	const FDateTime* GetTimestamp(FName TimestampId) const;

	TEnumAsByte<ETimestampStatus> GetStatus(FName TimestampId) const;
	TEnumAsByte<ETimestampCooldownStatus> GetCooldownStatus(FName TimestampId, bool bOnlyOnce, const FTimespan& CooldownTime) const;

protected:

	virtual void HandleStorageLoaded();

	TWeakInterfacePtr<ITimestampProviderInterface> TimestampInterface;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

};

