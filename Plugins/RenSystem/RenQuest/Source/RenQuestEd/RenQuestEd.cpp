// Copyright Epic Games, Inc. All Rights Reserved.

// Parent Header
#include "RenQuestEd.h"

// Project Headers
#include "RenQuestEd/Public/StoryboardEdAction.h"
#include "RenQuestEd/Public/QuestEdAction.h"



#define LOCTEXT_NAMESPACE "FRenQuestEdModule"

void FRenQuestEdModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FAssetToolsModule::GetModule().Get();
		EAssetTypeCategories::Type AssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("REN_CLASSES")), FText::FromString(TEXT("Ren Classes")));
		
		StoryboardEdAction = MakeShareable(new FStoryboardEdAction(AssetCategory));
		QuestEdAction = MakeShareable(new FQuestEdAction(AssetCategory));

		AssetTools.RegisterAssetTypeActions(StoryboardEdAction.ToSharedRef());
		AssetTools.RegisterAssetTypeActions(QuestEdAction.ToSharedRef());
	}
}

void FRenQuestEdModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FAssetToolsModule::GetModule().Get();
		AssetTools.UnregisterAssetTypeActions(StoryboardEdAction.ToSharedRef());
		AssetTools.UnregisterAssetTypeActions(QuestEdAction.ToSharedRef());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRenQuestEdModule, RenQuestEd)

