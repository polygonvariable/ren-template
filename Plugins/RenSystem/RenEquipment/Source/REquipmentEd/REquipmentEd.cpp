// Copyright Epic Games, Inc. All Rights Reserved.

#include "REquipmentEd.h"
#include "EquipmentSlotIdCustomization.h"
#include "EquipmentSocketDefinitionCustomization.h"

#define LOCTEXT_NAMESPACE "FREquipmentEdModule"

void FREquipmentEdModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    
    PropertyModule.RegisterCustomPropertyTypeLayout("EquipmentSlotId", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FEquipmentSlotIdCustomization::MakeInstance));
    PropertyModule.RegisterCustomPropertyTypeLayout("EquipmentSocketDefinition", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FEquipmentSocketDefinitionCustomization::MakeInstance));
    PropertyModule.NotifyCustomizationModuleChanged();
}

void FREquipmentEdModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
    if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
    {
        FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
        PropertyModule.UnregisterCustomPropertyTypeLayout("EquipmentSlotId");
        PropertyModule.UnregisterCustomPropertyTypeLayout("EquipmentSocketDefinition");
    }
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FREquipmentEdModule, REquipmentEd)

