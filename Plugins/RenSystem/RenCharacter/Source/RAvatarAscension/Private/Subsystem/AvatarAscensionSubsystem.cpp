// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/AvatarAscensionSubsystem.h"

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Subsystem/AuthActionSubsystem.h"
#include "Task/Task_GrantAvatarExperience.h"
#include "Task/Task_GrantAvatarRank.h"


bool UAvatarAscensionSubsystem::TryAddExperiencePoints(FName TargetSourceId, FPrimaryAssetId TargetAssetId, FPrimaryAssetId MaterialAssetId, FGuid MaterialId)
{
	UAuthActionSubsystem* AuthActionSubsystem = UAuthActionSubsystem::Get(GetGameInstance());
	if (!IsValid(AuthActionSubsystem))
	{
		return false;
	}

	FGuid ActionId = FGuid::NewGuid();
	UTask_GrantAvatarExperience* Action = AuthActionSubsystem->CreateAction<UTask_GrantAvatarExperience>(ActionId);
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
	UTask_GrantAvatarRank* Action = AuthActionSubsystem->CreateAction<UTask_GrantAvatarRank>(ActionId);
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

