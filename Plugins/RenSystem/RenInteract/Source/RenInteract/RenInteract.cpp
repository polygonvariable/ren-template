// Copyright Epic Games, Inc. All Rights Reserved.

#include "RenInteract.h"

#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

#define LOCTEXT_NAMESPACE "FRenInteractModule"



TSharedPtr<FSlateStyleSet> FRenInteractModule::StyleSet;

void FRenInteractModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	if (StyleSet.IsValid())
	{
		return;
	}

	StyleSet = MakeShared<FSlateStyleSet>(FName("RenInteractStyle"));

	SetIconAndThumbnail(TEXT("InteractActor"), TEXT("RenInteract_Actor128.png"));
	SetIconAndThumbnail(TEXT("InteractWidget"), TEXT("RenInteract_Widget128.png"));
	SetIconAndThumbnail(TEXT("InteractEntryWidget"), TEXT("RenInteract_Widget128.png"));

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

void FRenInteractModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}

void FRenInteractModule::SetIconAndThumbnail(FString ClassName, FString ImagePath)
{
	FString ThumbnailClass = TEXT("ClassThumbnail.") + ClassName;
	FString IconClass = TEXT("ClassIcon.") + ClassName;

	FString IconPath = IPluginManager::Get().FindPlugin(TEXT("RenInteract"))->GetBaseDir() + TEXT("/Resources/") + ImagePath;

	StyleSet->Set(FName(*ThumbnailClass), new FSlateImageBrush(IconPath, FVector2D(64.0f, 64.0f)));
	StyleSet->Set(FName(*IconClass), new FSlateImageBrush(IconPath, FVector2D(16.0f, 16.0f)));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRenInteractModule, RenInteract)

