// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "TimestampCooldownComponent.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenTimestamp/Public/TimestampSubsystem.h"



ETimestampCooldownStatus UTimestampCooldownComponent::GetCooldownStatus() const
{
	UTimestampSubsystem* TimestampSubsystemPtr = TimestampSubsystem.Get();
	if (!IsValid(TimestampSubsystemPtr))
	{
		return ETimestampCooldownStatus::NotFound;
	}

	return TimestampSubsystemPtr->GetCooldownStatus(ContainerId, TimestampId, bOnlyOnce, CooldownTime);
}

