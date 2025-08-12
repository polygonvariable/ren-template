// Copyright Epic Games, Inc. All Rights Reserved.

#include "RenAsset.h"

#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FRenAssetModule"

TSharedPtr<FSlateStyleSet> FRenAssetModule::StyleSet;

void FRenAssetModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	if (StyleSet.IsValid())
	{
		return;
	}

	StyleSet = MakeShared<FSlateStyleSet>(FName("RenAssetStyle"));

	SetIconAndThumbnail(TEXT("InventoryAsset"), TEXT("RenInventory_Icon128.png"));

	SetIconAndThumbnail(TEXT("CraftableAsset"), TEXT("RenInventory_Anvil128.png"));
	SetIconAndThumbnail(TEXT("EnhanceableAsset"), TEXT("RenInventory_LevelUp128.png"));

	SetIconAndThumbnail(TEXT("CurrencyAsset"), TEXT("RenInventory_Coin128.png"));
	SetIconAndThumbnail(TEXT("EnhanceAsset"), TEXT("RenInventory_Potion128.png"));
	SetIconAndThumbnail(TEXT("FoodAsset"), TEXT("RenInventory_Food128.png"));
	SetIconAndThumbnail(TEXT("MaterialAsset"), TEXT("RenInventory_Crystal128.png"));
	SetIconAndThumbnail(TEXT("WeaponAsset"), TEXT("RenInventory_Sword128.png"));

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

void FRenAssetModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}

void FRenAssetModule::SetIconAndThumbnail(FString ClassName, FString ImagePath)
{
	FString ThumbnailClass = TEXT("ClassThumbnail.") + ClassName;
	FString IconClass = TEXT("ClassIcon.") + ClassName;

	FString IconPath = IPluginManager::Get().FindPlugin(TEXT("RenAsset"))->GetBaseDir() + TEXT("/Resources/") + ImagePath;

	StyleSet->Set(FName(*ThumbnailClass), new FSlateImageBrush(IconPath, FVector2D(64.0f, 64.0f)));
	StyleSet->Set(FName(*IconClass), new FSlateImageBrush(IconPath, FVector2D(16.0f, 16.0f)));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRenAssetModule, RenAsset)

