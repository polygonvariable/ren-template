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

class UCounterSubsystem;



/**
 *
 */
UCLASS()
class UTimestampSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	bool AddTimestamp(FName ContainerId, FName TimestampId, bool bOverride);
	bool RemoveTimestamp(FName ContainerId, FName TimestampId);
	bool ContainsTimestamp(FName ContainerId, FName TimestampId) const;

	FDateTime GetTimestamp(FName ContainerId, FName TimestampId) const;

	ETimestampStatus GetStatus(FName ContainerId, FName TimestampId) const;
	ETimestampCooldownStatus GetCooldownStatus(FName ContainerId, FName TimestampId, bool bOnlyOnce, const FTimespan& CooldownTime) const;

protected:

	TWeakObjectPtr<UCounterSubsystem> CounterSubsystem = nullptr;
	TWeakInterfacePtr<ITimestampProviderInterface> TimestampInterface;

	virtual void HandleGameInitialized();

protected:

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

};

