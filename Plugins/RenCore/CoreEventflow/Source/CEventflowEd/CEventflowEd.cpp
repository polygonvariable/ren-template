// Copyright Epic Games, Inc. All Rights Reserved.

#include "CEventflowEd.h"

#include "EdGraphUtilities.h"
#include "IAssetTools.h"

#include "EventflowEdAction.h"
#include "EventflowTransitionCustomization.h"
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

    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    
    PropertyModule.RegisterCustomPropertyTypeLayout("EventflowTransition", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FEventflowTransitionCustomization::MakeInstance));
    PropertyModule.NotifyCustomizationModuleChanged();
}

void FCEventflowEdModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEdGraphUtilities::UnregisterVisualPinFactory(PinFactory);

	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
    {
        FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
        PropertyModule.UnregisterCustomPropertyTypeLayout("EventflowTransition");
    }
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCEventflowEdModule, CEventflowEd)

