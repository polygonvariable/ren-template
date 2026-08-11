// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/AvatarAscensionSubsystem.h"

// Project Headers
#include "Auth/AAGrantAvatarExperience.h"
#include "Auth/AAGrantAvatarRank.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Subsystem/AuthActionSubsystem.h"


bool UAvatarAscensionSubsystem::TryAddExperiencePoints(FName TargetSourceId, FPrimaryAssetId TargetAssetId, FPrimaryAssetId MaterialAssetId, FGuid MaterialId)
{
	UAuthActionSubsystem* AuthActionSubsystem = UAuthActionSubsystem::Get(GetGameInstance());
	if (!IsValid(AuthActionSubsystem))
	{
		return false;
	}

	FGuid ActionId = FGuid::NewGuid();
	UAAGrantAvatarExperience* Action = AuthActionSubsystem->CreateAction<UAAGrantAvatarExperience>(ActionId);
	if (!IsValid(Action))
	{
		return false;
	}

	Action->TargetSourceId = TargetSourceId;
	Action->TargetAssetId = TargetAssetId;
	Action->MaterialAssetId = MaterialAssetId;
	Action->MaterialId = MaterialId;
	return Action->StartAction();
}

bool UAvatarAscensionSubsystem::TryAddRankPoints(FName TargetSourceId, FPrimaryAssetId TargetAssetId)
{
	UAuthActionSubsystem* AuthActionSubsystem = UAuthActionSubsystem::Get(GetGameInstance());
	if (!IsValid(AuthActionSubsystem))
	{
		return false;
	}

	FGuid ActionId = FGuid::NewGuid();
	UAAGrantAvatarRank* Action = AuthActionSubsystem->CreateAction<UAAGrantAvatarRank>(ActionId);
	if (!IsValid(Action))
	{
		return false;
	}

	Action->TargetSourceId = TargetSourceId;
	Action->TargetAssetId = TargetAssetId;
	return Action->StartAction();
}

bool UAvatarAscensionSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UAvatarAscensionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogInventoryAscension, TEXT("AvatarAscensionSubsystem initialized"));
}

void UAvatarAscensionSubsystem::Deinitialize()
{
	LOG_WARNING(LogInventoryAscension, TEXT("AvatarAscensionSubsystem deinitialized"));
	Super::Deinitialize();
}

UAvatarAscensionSubsystem* UAvatarAscensionSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UAvatarAscensionSubsystem* UAvatarAscensionSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UAvatarAscensionSubsystem>();
}

