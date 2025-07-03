// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/RenGameInstanceSubsystem.h"

// Project Headers
#include "Developer/GameMetadataSettings.h"
#include "RenCore/Public/Macro/LogMacro.h"


void URenGameInstanceSubsystem::PostInitialize_Implementation()
{
	LOG_WARNING(LogTemp, "PostInitialize");
}

void URenGameInstanceSubsystem::OnInitialized_Implementation()
{
	LOG_WARNING(LogTemp, "OnInitialized");
}

void URenGameInstanceSubsystem::OnDeinitialized_Implementation()
{
	LOG_WARNING(LogTemp, "OnDeinitialized");
}

void URenGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Execute_OnInitialized(this);
}

void URenGameInstanceSubsystem::Deinitialize()
{
	Execute_OnDeinitialized(this);
	Super::Deinitialize();
}

bool URenGameInstanceSubsystem::ShouldCreateSubsystem(UObject* Object) const
{
	if (!Object)
	{
		LOG_ERROR(LogTemp, "Object is null");
		return false;
	}

	const UGameMetadataSettings* GameMetadata = GetDefault<UGameMetadataSettings>();
	if (!GameMetadata)
	{
		LOG_ERROR(LogTemp, "GameMetadataSettings is null");
		return false;
	}
	
	for (auto& SubsystemClass : GameMetadata->SubsystemClasses)
	{
		if (SubsystemClass == this->GetClass())
		{
			LOG_WARNING(LogTemp, "Subsystem found");
			return true;
		}
	}

	LOG_ERROR(LogTemp, "Subsystem not found");
	return false;
}

UWorld* URenGameInstanceSubsystem::GetWorld() const
{
	return nullptr;
}

