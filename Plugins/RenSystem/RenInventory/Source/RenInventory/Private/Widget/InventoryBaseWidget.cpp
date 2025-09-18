// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryBaseWidget.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "RCoreInventory/Public/InventoryAsset.h"



void UInventoryBaseWidget::InitializeDetails(const FPrimaryAssetId& AssetId, const FName& RecordId, const FInventoryRecord* Record)
{

}

void UInventoryBaseWidget::RefreshDetails()
{

}

void UInventoryBaseWidget::ResetDetails()
{

}

void UInventoryBaseWidget::SetPrimaryDetails(const FText& Title, const FText& Description, const TSoftObjectPtr<UTexture2D>& Image)
{

}

void UInventoryBaseWidget::SetSecondaryDetails(const FText& RecordId, int Quantity)
{

}

void UInventoryBaseWidget::SetTertiaryDetails(UInventoryEntryObject* Entry)
{

}

void UInventoryBaseWidget::NativeConstruct()
{
	AssetManager = UAssetManager::GetIfInitialized();
	Super::NativeConstruct();
}

void UInventoryBaseWidget::NativeDestruct()
{
	AssetManager = nullptr;
	Super::NativeDestruct();
}

