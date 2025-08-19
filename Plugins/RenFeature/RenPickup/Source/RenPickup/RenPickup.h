// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Styling/SlateStyle.h"



class FRenPickupModule : public IModuleInterface
{
	
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

#if WITH_EDITOR

	static TSharedPtr<FSlateStyleSet> StyleSet;
	
	void SetIconAndThumbnail(FString ClassName, FString ImagePath);

#endif

};

