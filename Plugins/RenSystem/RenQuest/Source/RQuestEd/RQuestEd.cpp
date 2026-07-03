// Copyright Epic Games, Inc. All Rights Reserved.

// Parent Header
#include "RQuestEd.h"

// Project Headers
#include "QuestEdAssetTypeAction.h"


#define LOCTEXT_NAMESPACE "FRQuestEdModule"

void FRQuestEdModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FAssetToolsModule::GetModule().Get();
		EAssetTypeCategories::Type AssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("REN_CLASSES")), FText::FromString(TEXT("Ren Classes")));
		
		EdAction = MakeShareable(new FQuestEdAssetTypeAction(AssetCategory));
		AssetTools.RegisterAssetTypeActions(EdAction.ToSharedRef());
	}
}

void FRQuestEdModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FAssetToolsModule::GetModule().Get();
		AssetTools.UnregisterAssetTypeActions(EdAction.ToSharedRef());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRQuestEdModule, RQuestEd)

