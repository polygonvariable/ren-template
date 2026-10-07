// Copyright Epic Games, Inc. All Rights Reserved.

// Parent Header
#include "CEventflowEd.h"

// Engine Headers
#include "EdGraphUtilities.h"
#include "IAssetTools.h"

// Project Headers
#include "EventflowEdAction.h"
#include "Graph/EventflowEdGraphPin.h"

#define LOCTEXT_NAMESPACE "FCEventflowEdModule"

void FCEventflowEdModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	IAssetTools& AssetTools = IAssetTools::Get();
	EAssetTypeCategories::Type Category = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("REN_CLASSES")), FText::FromString(TEXT("Ren Classes")));

	TSharedPtr<FEventflowEdAction> Action = MakeShareable(new FEventflowEdAction(Category));
	AssetTools.RegisterAssetTypeActions(Action.ToSharedRef());

	PinFactory = MakeShareable(new FEventflowEdPanelPinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(PinFactory);
}

void FCEventflowEdModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEdGraphUtilities::UnregisterVisualPinFactory(PinFactory);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCEventflowEdModule, CEventflowEd)

