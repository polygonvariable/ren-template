// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/InventoryAscensionSubsystem.h"

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Subsystem/AuthActionSubsystem.h"
#include "Task/Task_GrantItemExperience.h"
#include "Task/Task_GrantItemRank.h"


bool UInventoryAscensionSubsystem::TryAddExperiencePoints(FName SourceId, FPrimaryAssetId TargetAssetId, FGuid TargetId, FPrimaryAssetId MaterialAssetId, FGuid MaterialId)
{
	UAuthActionSubsystem* AuthActionSubsystem = UAuthActionSubsystem::Get(GetGameInstance());
	if (!IsValid(AuthActionSubsystem))
	{
		return false;
	}

	FGuid ActionId = FGuid::NewGuid();
	UTask_GrantItemExperience* Action = AuthActionSubsystem->CreateAction<UTask_GrantItemExperience>(ActionId);
	if (!IsValid(Action))
	{
		return false;
	}

	Action->SourceId = SourceId;
	Action->TargetAssetId = TargetAssetId;
	Action->TargetId = TargetId;
	Action->MaterialAssetId = MaterialAssetId;
	Action->MaterialId = MaterialId;
	return Action->StartAction();
}

bool UInventoryAscensionSubsystem::TryAddRankPoints(FName SourceId, FPrimaryAssetId TargetAssetId, FGuid TargetId)
{
	UAuthActionSubsystem* AuthActionSubsystem = UAuthActionSubsystem::Get(GetGameInstance());
	if (!IsValid(AuthActionSubsystem))
	{
		return false;
	}

	FGuid ActionId = FGuid::NewGuid();
	UTask_GrantItemRank* Action = AuthActionSubsystem->CreateAction<UTask_GrantItemRank>(ActionId);
	if (!IsValid(Action))
	{
		return false;
	}

	Action->SourceId = SourceId;
	Action->TargetAssetId = TargetAssetId;
	Action->TargetId = TargetId;
	return Action->StartAction();
}

bool UInventoryAscensionSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UInventoryAscensionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogInventoryAscension, TEXT("InventoryAscensionSubsystem initialized"));
}

void UInventoryAscensionSubsystem::Deinitialize()
{
	LOG_WARNING(LogInventoryAscension, TEXT("InventoryAscensionSubsystem deinitialized"));
	Super::Deinitialize();
}

UInventoryAscensionSubsystem* UInventoryAscensionSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UInventoryAscensionSubsystem* UInventoryAscensionSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UInventoryAscensionSubsystem>();
}

