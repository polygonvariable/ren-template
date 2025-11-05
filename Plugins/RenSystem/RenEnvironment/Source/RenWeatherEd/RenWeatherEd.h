// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

// Forward Declaration
class FSlateStyleSet;
class FAssetTypeActions_Base;



/**
 *
 * 
 */
class FRenWeatherEdModule : public IModuleInterface
{

public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	TSharedPtr<FSlateStyleSet> StyleSet;
	TArray<TSharedPtr<FAssetTypeActions_Base>> AssetActions;

	void SetIconAndThumbnail(FString ClassName, FString ImagePath);

};

