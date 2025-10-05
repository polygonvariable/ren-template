// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryUI.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers



void UInventoryUI::InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity)
{
}

void UInventoryUI::InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record)
{
}

void UInventoryUI::RefreshDetails()
{
}

void UInventoryUI::ResetDetails()
{
}

void UInventoryUI::SetPrimaryDetails(const FText& Title, const FText& Description, const TSoftObjectPtr<UTexture2D>& Image)
{
}

void UInventoryUI::SetSecondaryDetails(int Quantity)
{
}

void UInventoryUI::SetSecondaryDetails(int Quantity, const FEnhanceRecord& Enhance)
{
}

void UInventoryUI::SetTertiaryDetails(UInventoryEntry* Entry)
{
}

void UInventoryUI::LockControls(bool bLock)
{

}

void UInventoryUI::LockControls(TArray<UWidget*> Widgets, bool bLock)
{
	for (UWidget* Widget : Widgets)
	{
		Widget->SetIsEnabled(!bLock);
	}
}

void UInventoryUI::CloseWidget()
{
	RemoveFromParent();
}

void UInventoryUI::SwitchDetails(bool bPrimary)
{
}

void UInventoryUI::SwitchDetails(int Index)
{
}

void UInventoryUI::NativeConstruct()
{
	AssetManager = UAssetManager::GetIfInitialized();
	Super::NativeConstruct();
}

void UInventoryUI::NativeDestruct()
{
	AssetManager = nullptr;
	Super::NativeDestruct();
}

