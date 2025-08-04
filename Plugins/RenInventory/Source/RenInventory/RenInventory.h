// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FRenInventoryModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

#if WITH_EDITOR

	TArray<IConsoleCommand*> Commands;

	void RegisterCommand();
	void UnregisterCommand();

	void AddItem(const TArray<FString>& Args, UWorld* World);
	void RemoveItem(const TArray<FString>& Args, UWorld* World);

#endif
};

