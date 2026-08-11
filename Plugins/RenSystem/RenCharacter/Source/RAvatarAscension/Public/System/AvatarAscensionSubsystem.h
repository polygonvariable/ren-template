// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Generated Headers
#include "AvatarAscensionSubsystem.generated.h"

// Module Macros
#define REN_API RAVATARASCENSION_API


/**
 *
 */
UCLASS(NotBlueprintType, MinimalAPI)
class UAvatarAscensionSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	REN_API bool TryAddExperiencePoints(FName TargetSourceId, FPrimaryAssetId TargetAssetId, FPrimaryAssetId MaterialAssetId, FGuid MaterialId);
	REN_API bool TryAddRankPoints(FName TargetSourceId, FPrimaryAssetId TargetAssetId);

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

