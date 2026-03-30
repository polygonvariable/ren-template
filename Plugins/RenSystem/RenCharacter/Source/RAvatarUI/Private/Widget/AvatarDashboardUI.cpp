// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarDashboardUI.h"

// Project Headers
#include "Widget/AssetCollectionUI.h"
#include "Widget/AssetDetailUI.h"
#include "Widget/AssetEntry.h"



void UAvatarDashboardUI::InitializeDetail()
{
	AvatarDetail->PrimarySourceId = PrimarySourceId;
	AvatarDetail->InitializeDetail();

	AvatarCollection->PrimarySourceId = PrimarySourceId;
	AvatarCollection->InitializeCollection();
	AvatarCollection->DisplayEntries();
}

void UAvatarDashboardUI::ResetDetail()
{
	AvatarDetail->ResetDetail();
}

void UAvatarDashboardUI::SetPrimaryDetail(const UCoreDataAsset* Asset)
{
	AvatarDetail->InitializeAssetDetail(Asset);
}

void UAvatarDashboardUI::SetSecondaryDetail(const UAssetEntry* Entry)
{
	AvatarDetail->InitializeEntryDetail(Entry);
}

void UAvatarDashboardUI::RedirectToWidget(TSubclassOf<UAssetDashboardUI> WidgetClass)
{
	const UAssetEntry* Entry = AvatarCollection->GetSelectedEntry();
	if (!IsValid(WidgetClass) || !IsValid(Entry))
	{
		return;
	}

	UAssetDashboardUI* Widget = CreateWidget<UAssetDashboardUI>(this, WidgetClass);
	if (!IsValid(Widget))
	{
		return;
	}

	Widget->PrimarySourceId = PrimarySourceId;
	Widget->AddToViewport();
	Widget->InitializeDetail();
	Widget->InitializeAssetByEntry(Entry);
}

void UAvatarDashboardUI::NativeConstruct()
{
	AvatarCollection->OnSelectionChanged.BindUObject(this, &UAssetDashboardUI::InitializeAssetByEntry);
	AvatarCollection->OnSelectionCleared.BindUObject(this, &UAssetDashboardUI::ResetDetail);

	Super::NativeConstruct();
}

void UAvatarDashboardUI::NativeDestruct()
{
	AvatarCollection->OnSelectionChanged.Unbind();
	AvatarCollection->OnSelectionCleared.Unbind();

	Super::NativeDestruct();
}

