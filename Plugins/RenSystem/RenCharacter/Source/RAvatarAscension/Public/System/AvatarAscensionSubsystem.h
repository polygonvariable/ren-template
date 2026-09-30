// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Generated Headers
#include "AvatarAscensionSubsystem.generated.h"


/**
 *
 */
UCLASS(NotBlueprintType, MinimalAPI)
class UAvatarAscensionSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	RAVATARASCENSION_API bool TryAddExperiencePoints(FName TargetSourceId, FPrimaryAssetId TargetAssetId, FPrimaryAssetId MaterialAssetId, FGuid MaterialId);
	RAVATARASCENSION_API bool TryAddRankPoints(FName TargetSourceId, FPrimaryAssetId TargetAssetId);

protected:

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	RAVATARASCENSION_API static UAvatarAscensionSubsystem* Get(UWorld* World);
	RAVATARASCENSION_API static UAvatarAscensionSubsystem* Get(UGameInstance* GameInstance);

};

