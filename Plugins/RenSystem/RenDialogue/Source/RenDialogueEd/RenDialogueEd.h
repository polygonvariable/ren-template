// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

// Forward declaration
class FDialogueEdAssetTypeAction;



class FRenDialogueEdModule : public IModuleInterface
{

public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	TSharedPtr<FDialogueEdAssetTypeAction> DialogueEdAction;

};

