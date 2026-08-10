// Copyright Epic Games, Inc. All Rights Reserved.

#include "RCoreLuauEd.h"
#include "LuauSourceCodeCustomization.h"

#define LOCTEXT_NAMESPACE "FRCoreLuauEdModule"

void FRCoreLuauEdModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    
    PropertyModule.RegisterCustomPropertyTypeLayout("LuauSourceCode", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FLuauSourceCodeCustomization::MakeInstance));
    PropertyModule.NotifyCustomizationModuleChanged();
}

void FRCoreLuauEdModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
    if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
    {
        FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
        PropertyModule.UnregisterCustomPropertyTypeLayout("LuauSourceCode");
    }
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRCoreLuauEdModule, RCoreLuauEd)

