// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// Engine headers
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"



/*
 * 
 */
class FRenPCGModule : public IModuleInterface
{

public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};

