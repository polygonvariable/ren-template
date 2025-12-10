// Copyright Epic Games, Inc. All Rights Reserved.

// Parent Header
#include "RenEnvironmentEd.h"

// Engine Headers
#include "IAssetTools.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

// Project Headers
#include "RenEnvironmentEd/Public/EnvironmentEdAction.h"



#define LOCTEXT_NAMESPACE "FRenEnvironmentEdModule"

void FRenEnvironmentEdModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FModuleManager& ModuleManager = FModuleManager::Get();
	if (ModuleManager.IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		
		TSharedPtr<FEnvironmentProfileActions> EnvironmentProfile = MakeShareable(new FEnvironmentProfileActions());
		TSharedPtr<FEnvironmentFogProfileActions> FogProfile = MakeShareable(new FEnvironmentFogProfileActions());
		TSharedPtr<FEnvironmentLightProfileActions> LightProfile = MakeShareable(new FEnvironmentLightProfileActions());
		TSharedPtr<FEnvironmentAtmosphereProfileActions> AtmosphereProfile = MakeShareable(new FEnvironmentAtmosphereProfileActions());

		TSharedPtr<FEnvironmentAssetActions> EnvironmentAction = MakeShareable(new FEnvironmentAssetActions());

		AssetActions.Push(EnvironmentProfile);
		AssetActions.Push(FogProfile);
		AssetActions.Push(LightProfile);
		AssetActions.Push(AtmosphereProfile);
		AssetActions.Push(EnvironmentAction);

		for (TSharedPtr<IAssetTypeActions> Action : AssetActions)
		{
			AssetTools.RegisterAssetTypeActions(Action.ToSharedRef());
		}
	}

	if (StyleSet.IsValid())
	{
		return;
	}

	StyleSet = MakeShared<FSlateStyleSet>(FName("RenEnvironmentStyle"));

	SetIconAndThumbnail(TEXT("EnvironmentAsset"), TEXT("EnvironmentAsset128.png"));
	SetIconAndThumbnail(TEXT("EnvironmentProfileAsset"), TEXT("EnvironmentAsset128.png"));

	SetIconAndThumbnail(TEXT("EnvironmentFogProfileAsset"), TEXT("ProfileFog128.png"));
	SetIconAndThumbnail(TEXT("EnvironmentLightProfileAsset"), TEXT("ProfileLight128.png"));
	SetIconAndThumbnail(TEXT("EnvironmentAtmosphereProfileAsset"), TEXT("ProfileAtmosphere128.png"));

	SetIconAndThumbnail(TEXT("EnvironmentActor"), TEXT("ActorEnvironment128.png"));
	SetIconAndThumbnail(TEXT("EnvironmentNiagaraCanvas"), TEXT("ActorNiagaraPaint128.png"));
	SetIconAndThumbnail(TEXT("EnvironmentMaterialCanvas"), TEXT("ActorMaterialPaint128.png"));

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

void FRenEnvironmentEdModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FModuleManager& ModuleManager = FModuleManager::Get();
	if (ModuleManager.IsModuleLoaded("AssetTools"))
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

void FRenEnvironmentEdModule::SetIconAndThumbnail(FString ClassName, FString ImagePath)
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

IMPLEMENT_MODULE(FRenEnvironmentEdModule, RenEnvironmentEd)

