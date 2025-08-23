// Copyright Epic Games, Inc. All Rights Reserved.

#include "RenEventflowEditor.h"

#include "IAssetTools.h"

#include "EdGraphUtilities.h"
#include "KismetPins/SGraphPinColor.h"
#include "EdGraph/EdGraphPin.h"

#include "RenEventflowEditor/Public/EventflowEdAssetTypeAction.h"
#include "RenEventflowEditor/Public/EventflowEdGraphPin.h"
//#include "AssetToolsModules.h"


#define LOCTEXT_NAMESPACE "FRenEventflowEditorModule"

void FRenEventflowEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	IAssetTools& AssetTools = IAssetTools::Get();
	EAssetTypeCategories::Type Category = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("REN_GRAPH")), FText::FromString(TEXT("Ren Classes")));

	TSharedPtr<EventflowEdAssetTypeAction> Action = MakeShareable(new EventflowEdAssetTypeAction(Category));
	AssetTools.RegisterAssetTypeActions(Action.ToSharedRef());

	PinFactory = MakeShareable(new FEventflowEdPanelPinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(PinFactory);
}

void FRenEventflowEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FEdGraphUtilities::UnregisterVisualPinFactory(PinFactory);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRenEventflowEditorModule, RenEventflowEditor)

