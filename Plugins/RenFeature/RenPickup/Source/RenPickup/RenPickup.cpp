// Copyright Epic Games, Inc. All Rights Reserved.

#include "RenPickup.h"

#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

#define LOCTEXT_NAMESPACE "FRenPickupModule"



#if WITH_EDITOR
TSharedPtr<FSlateStyleSet> FRenPickupModule::StyleSet;
#endif

void FRenPickupModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

#if WITH_EDITOR
	if (!StyleSet.IsValid())
	{
		return;
	}

	StyleSet = MakeShared<FSlateStyleSet>(FName("RenPickupStyle"));
	
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
#endif

}

void FRenPickupModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

#if WITH_EDITOR
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
#endif
}

#if WITH_EDITOR
void FRenPickupModule::SetIconAndThumbnail(FString ClassName, FString ImagePath)
{
	FString ThumbnailClass = TEXT("ClassThumbnail.") + ClassName;
	FString IconClass = TEXT("ClassIcon.") + ClassName;

	FString IconPath = IPluginManager::Get().FindPlugin(TEXT("RenPickup"))->GetBaseDir() + TEXT("/Resources/") + ImagePath;

	StyleSet->Set(FName(*ThumbnailClass), new FSlateImageBrush(IconPath, FVector2D(64.0f, 64.0f)));
	StyleSet->Set(FName(*IconClass), new FSlateImageBrush(IconPath, FVector2D(16.0f, 16.0f)));
}
#endif

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRenPickupModule, RenPickup)

