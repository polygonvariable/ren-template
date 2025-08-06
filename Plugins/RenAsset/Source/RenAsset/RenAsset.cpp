// Copyright Epic Games, Inc. All Rights Reserved.

#include "RenAsset.h"

#include "Styling/SlateStyle.h"
// #include "Styling/SlateStyleRegistry.h"
// #include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FRenAssetModule"

// TSharedPtr<FSlateStyleSet> FRenAssetModule::StyleSet;

void FRenAssetModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	/*if (StyleSet.IsValid())
	{
		return;
	}

	StyleSet = MakeShared<FSlateStyleSet>(FName("RenAssetStyle"));

	FString IconPath = IPluginManager::Get().FindPlugin(TEXT("RenAsset"))->GetBaseDir() + TEXT("/Resources/net_64.png");
	StyleSet->Set("ClassThumbnail.InventoryAsset", new FSlateImageBrush(IconPath, FVector2D(40.0f, 40.0f)));
	StyleSet->Set("ClassIcon.InventoryAsset", new FSlateImageBrush(IconPath, FVector2D(16.0f, 16.0f)));

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());*/
}

void FRenAssetModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	/*if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}*/
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRenAssetModule, RenAsset)

