// Copyright Epic Games, Inc. All Rights Reserved.

// Parent Header
#include "REnemyPartyEd.h"

// Project Headers
#include "EnemyEdMode.h"

#define LOCTEXT_NAMESPACE "FREnemyPartyEdModule"

void FREnemyPartyEdModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

    FEditorModeRegistry::Get().RegisterMode<FEnemyEdMode>(
        FEnemyEdMode::EM_EnemyManager,
        FText::FromString("Enemy Editor"),
        FSlateIcon(),
        true,
        100
    );

	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
	TSharedRef<FPropertySection> Section = PropertyModule.FindOrCreateSection(TEXT("EnemyManagerComponent"), TEXT("Collection"), FText::FromString("Spawn Data"));
	Section->AddCategory(TEXT("Collection"));
	Section->AddCategory(TEXT("Source"));
}

void FREnemyPartyEdModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FEditorModeRegistry::Get().UnregisterMode(FEnemyEdMode::EM_EnemyManager);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FREnemyPartyEdModule, REnemyPartyEd)
