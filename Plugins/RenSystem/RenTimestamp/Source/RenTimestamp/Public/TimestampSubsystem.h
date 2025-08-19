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

	bool AddTimestamp(FName Guid, bool bForceAdd);
	bool RemoveTimestamp(FName Guid);
	bool ContainsTimestamp(FName Guid) const;

	const FDateTime* GetTimestamp(FName Guid) const;

	TEnumAsByte<ETimestampStatus> GetStatus(FName Guid) const;
	TEnumAsByte<ETimestampCooldownStatus> GetCooldownStatus(FName Guid, bool bOnlyOnce, const FTimespan& CooldownTime) const;

protected:

	virtual void HandleStorageLoaded();

	TWeakInterfacePtr<ITimestampProviderInterface> TimestampInterface;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

};

