// Copyright Epic Games, Inc. All Rights Reserved.

// Parent Header
#include "RenWeatherEd.h"

// Engine Headers
#include "IAssetTools.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

// Project Headers
#include "RenWeatherEd/Public/WeatherEdAction.h"



#define LOCTEXT_NAMESPACE "FRenWeatherEdModule"

void FRenWeatherEdModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

		TSharedPtr<FWeatherAssetActions> WeatherAsset = MakeShareable(new FWeatherAssetActions());
		AssetActions.Push(WeatherAsset);

		for (TSharedPtr<IAssetTypeActions> Action : AssetActions)
		{
			AssetTools.RegisterAssetTypeActions(Action.ToSharedRef());
		}
	}

	if (StyleSet.IsValid())
	{
		return;
	}

	StyleSet = MakeShared<FSlateStyleSet>(FName("RenWeatherStyle"));

	SetIconAndThumbnail(TEXT("WeatherAsset"), TEXT("Weather128.png"));

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

void FRenWeatherEdModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (TSharedPtr<IAssetTypeActions> Action : AssetActions)
		{
			AssetTools.UnregisterAssetTypeActions(Action.ToSharedRef());
		}
	}
	AssetActions.Empty();

	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}

void FRenWeatherEdModule::SetIconAndThumbnail(FString ClassName, FString ImagePath)
{
	FString ThumbnailClass = TEXT("ClassThumbnail.") + ClassName;
	FString IconClass = TEXT("ClassIcon.") + ClassName;

	FString BasePath = IPluginManager::Get().FindPlugin(TEXT("RenEnvironment"))->GetBaseDir() + TEXT("/Resources/");

	FString ThumbnailPath = BasePath + ImagePath;
	FString IconPath = BasePath + ImagePath;

	StyleSet->Set(FName(*ThumbnailClass), new FSlateImageBrush(ThumbnailPath, FVector2D(64.0f, 64.0f)));
	StyleSet->Set(FName(*IconClass), new FSlateImageBrush(IconPath, FVector2D(16.0f, 16.0f)));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRenWeatherEdModule, RenWeatherEd)

