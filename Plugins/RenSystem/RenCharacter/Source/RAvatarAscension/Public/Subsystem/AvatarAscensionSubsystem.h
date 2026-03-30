// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "Definition/TaskType.h"

// Generated Headers
#include "AvatarAscensionSubsystem.generated.h"

// Module Macros
#define REN_API RAVATARASCENSION_API

// Forward Declarations



/**
 *
 */
UCLASS(NotBlueprintType, MinimalAPI)
class UAvatarAscensionSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	REN_API void AddExperiencePoints(FName TargetSourceId, FPrimaryAssetId TargetAssetId, FPrimaryAssetId MaterialAssetId, FGuid MaterialId, FTaskCallback Callback);
	REN_API void AddRankPoints(FName TargetSourceId, FPrimaryAssetId TargetAssetId, FTaskCallback Callback);

protected:

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	REN_API static UAvatarAscensionSubsystem* Get(UWorld* World);
	REN_API static UAvatarAscensionSubsystem* Get(UGameInstance* GameInstance);

};



// Module Macros
#undef REN_API

