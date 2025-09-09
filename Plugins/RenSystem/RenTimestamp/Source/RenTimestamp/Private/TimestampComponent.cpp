// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "TimestampComponent.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenTimestamp/Public/TimestampSubsystem.h"



UTimestampComponent::UTimestampComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

TEnumAsByte<ETimestampStatus> UTimestampComponent::GetStatus() const
{
	UTimestampSubsystem* TimestampSubsystemPtr = TimestampSubsystem.Get();
	if (!IsValid(TimestampSubsystemPtr))
	{
		return ETimestampStatus::Invalid;
	}

	return TimestampSubsystemPtr->GetStatus(TimestampId);
}

bool UTimestampComponent::AddTimestamp(bool bForceAdd)
{
	UTimestampSubsystem* TimestampSubsystemPtr = TimestampSubsystem.Get();
	if (!IsValid(TimestampSubsystemPtr))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("TimestampSubsystem is invalid"));
		return false;
	}

	return TimestampSubsystemPtr->AddTimestamp(TimestampId, bForceAdd);
}


void UTimestampComponent::Activate(bool bReset)
{
	UWorld* World = GetWorld();
	if (!IsValid(World)) return;

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!IsValid(GameInstance)) return;

	TimestampSubsystem = GameInstance->GetSubsystem<UTimestampSubsystem>();

	Super::Activate(bReset);
}

void UTimestampComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TimestampSubsystem.Reset();

	Super::EndPlay(EndPlayReason);
}

