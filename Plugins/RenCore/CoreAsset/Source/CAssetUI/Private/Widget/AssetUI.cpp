// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AssetUI.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "Data/CoreDataAsset.h"
#include "Core/AssetManagerUtil.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Widget/AssetEntry.h"


void UAssetUI::InitializeAssetByEntry(const UAssetEntry* Entry)
{
	if (!IsValid(Entry))
	{
		LOG_ERROR(LogAsset, TEXT("Entry is invalid"));
		return;
	}

	InitializeEntryDetail(Entry);
	InitializeAssetById(Entry->AssetId);
}

void UAssetUI::InitializeAssetById(const FPrimaryAssetId& AssetId)
{
	if (!IsValid(AssetManager) || !AssetId.IsValid())
	{
		LOG_ERROR(LogAsset, TEXT("AssetManager or AssetId is invalid"));
		return;
	}

	if (_AssetId == AssetId)
	{
		InitializeAssetDetail(AssetManager->GetPrimaryAssetObject<UCoreDataAsset>(AssetId));
		return;
	}

	FAssetManagerUtil::CancelHandle(_AssetHandle);

	_AssetId = AssetId;
	_AssetHandle = AssetManager->LoadPrimaryAsset(AssetId, TArray<FName>(), FStreamableDelegate::CreateUObject(this, &UAssetUI::HandleAssetLoaded));
}

void UAssetUI::InitializeAssetDetail(const UCoreDataAsset* Asset)
{
	if (IsValid(Asset))
	{
		_AssetId = Asset->GetPrimaryAssetId();
	}
	SetPrimaryDetail(Asset);
}

void UAssetUI::InitializeEntryDetail(const UAssetEntry* Entry)
{
	SetSecondaryDetail(Entry);
}

TArray<UWidget*> UAssetUI::GetLockingControls_Implementation() const
{
	return TArray<UWidget*>();
}

const FPrimaryAssetId& UAssetUI::GetActiveAssetId() const
{
	return _AssetId;
}

const UCoreDataAsset* UAssetUI::GetActiveAsset() const
{
	return AssetManager->GetPrimaryAssetObject<UCoreDataAsset>(_AssetId);
}

void UAssetUI::CancelInitialization()
{
	FAssetManagerUtil::CancelHandle(_AssetHandle);
}

void UAssetUI::HandleAssetLoaded()
{
	FAssetManagerUtil::ReleaseHandle(_AssetHandle);

	UCoreDataAsset* Asset = AssetManager->GetPrimaryAssetObject<UCoreDataAsset>(_AssetId);
	InitializeAssetDetail(Asset);
}

void UAssetUI::CloseWidget()
{
	RemoveFromParent();
}

void UAssetUI::NativeConstruct()
{
	AssetManager = UAssetManager::GetIfInitialized();

	Super::NativeConstruct();
}

void UAssetUI::NativeDestruct()
{
	CancelInitialization();

	AssetManager = nullptr;

	Super::NativeDestruct();
}

