// Copyright Epic Games, Inc. All Rights Reserved.

// Parent Header
#include "RenDialogueEditor.h"

// Project Headers
#include "RenDialogueEditor/Public/DialogueEdAssetTypeAction.h"



#define LOCTEXT_NAMESPACE "FRenDialogueEditorModule"

void FRenDialogueEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FAssetToolsModule::GetModule().Get();
		EAssetTypeCategories::Type AssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("REN_CLASSES")), FText::FromString(TEXT("Ren Classes")));
		
		DialogueEdAction = MakeShareable(new FDialogueEdAssetTypeAction(AssetCategory));
		AssetTools.RegisterAssetTypeActions(DialogueEdAction.ToSharedRef());
	}
}

void FRenDialogueEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FAssetToolsModule::GetModule().Get();
		AssetTools.UnregisterAssetTypeActions(DialogueEdAction.ToSharedRef());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRenDialogueEditorModule, RenDialogueEditor)

