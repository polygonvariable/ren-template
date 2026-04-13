// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AssetUI.h"

// Project Headers
#include "Asset/CoreDataAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Manager/RAssetManager.inl"
#include "Widget/AssetEntry.h"


void UAssetUI::InitializeDetail()
{

}

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

	if (_ActiveAssetId == AssetId && IsValid(_ActiveAsset))
	{
		InitializeAssetDetail(_ActiveAsset.Get());
		return;
	}

	AssetManager->CancelFetch(_ActiveLoadId);

	_ActiveAssetId = AssetId;
	_ActiveLoadId = FGuid::NewGuid();

	TWeakObjectPtr<UAssetUI> WeakThis(this);
	TFuture<FLatentLoadedAsset<UCoreDataAsset>> Future = AssetManager->FetchPrimaryAsset<UCoreDataAsset>(_ActiveLoadId, _ActiveAssetId);
	Future.Next([WeakThis](const FLatentLoadedAsset<UCoreDataAsset>& Result)
		{
			UAssetUI* This = WeakThis.Get();
			if (IsValid(This) && Result.IsValid())
			{
				const UCoreDataAsset* Asset = Result.Get();

				This->_ActiveAsset = Asset;
				This->InitializeAssetDetail(Asset);
			}
		}
	);
}

void UAssetUI::InitializeAssetDetail(const UCoreDataAsset* Asset)
{
	if (IsValid(Asset))
	{
		_ActiveAssetId = Asset->GetPrimaryAssetId();
	}
	SetPrimaryDetail(Asset);
}

void UAssetUI::InitializeEntryDetail(const UAssetEntry* Entry)
{
	SetSecondaryDetail(Entry);
}

const FPrimaryAssetId& UAssetUI::GetActiveAssetId() const
{
	return _ActiveAssetId;
}

const UCoreDataAsset* UAssetUI::GetActiveAsset() const
{
	return _ActiveAsset.Get();
}

void UAssetUI::SetPrimaryDetail(const UCoreDataAsset* Asset)
{

}

void UAssetUI::SetSecondaryDetail(const UAssetEntry* Entry)
{

}

void UAssetUI::RefreshDetail()
{

}

void UAssetUI::ResetDetail()
{

}

void UAssetUI::CancelInitialization()
{
	if (_ActiveLoadId.IsValid() && IsValid(AssetManager))
	{
		AssetManager->CancelFetch(_ActiveLoadId);
	}
}

void UAssetUI::SwitchDetail(bool bPrimary)
{

}

void UAssetUI::CloseWidget()
{
	RemoveFromParent();
}

void UAssetUI::NativeConstruct()
{
	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());

	Super::NativeConstruct();
}

void UAssetUI::NativeDestruct()
{
	CancelInitialization();

	AssetManager = nullptr;

	Super::NativeDestruct();
}

