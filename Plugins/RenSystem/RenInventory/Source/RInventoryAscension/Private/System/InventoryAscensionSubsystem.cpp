// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/InventoryAscensionSubsystem.h"

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Subsystem/AuthActionSubsystem.h"
#include "Auth/AAGrantItemExperience.h"
#include "Auth/AAGrantItemRank.h"


bool UInventoryAscensionSubsystem::TryAddExperiencePoints(FName SourceId, FPrimaryAssetId TargetAssetId, FGuid TargetId, FPrimaryAssetId MaterialAssetId, FGuid MaterialId)
{
	UAuthActionSubsystem* AuthActionSubsystem = UAuthActionSubsystem::Get(GetGameInstance());
	if (!IsValid(AuthActionSubsystem))
	{
		return false;
	}

	FGuid ActionId = FGuid::NewGuid();
	UAAGrantItemExperience* Action = AuthActionSubsystem->CreateAction<UAAGrantItemExperience>(ActionId);
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
	UAAGrantItemRank* Action = AuthActionSubsystem->CreateAction<UAAGrantItemRank>(ActionId);
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

