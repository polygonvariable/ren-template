// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

// Forward declaration
class FStoryboardEdAction;
class FQuestEdAction;


class FRenQuestEdModule : public IModuleInterface
{

public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	TSharedPtr<FStoryboardEdAction> StoryboardEdAction;
	TSharedPtr<FQuestEdAction> QuestEdAction;

};

