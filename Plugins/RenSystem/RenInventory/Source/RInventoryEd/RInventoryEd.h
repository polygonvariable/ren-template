// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

// Forward Declaration
class UInventoryStorageManager;



class FRInventoryEdModule : public IModuleInterface
{

public:

	// ~ IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// ~ End of IModuleInterface

protected:

	TArray<IConsoleCommand*> Commands;

	void RegisterCommand();
	void UnregisterCommand();

	UInventoryStorageManager* GetStorageManager(UWorld* World, FName ContainerId);

	void AddItem(const TArray<FString>& Args, UWorld* World);
	void RemoveItem(const TArray<FString>& Args, UWorld* World);
	void GetItem(const TArray<FString>& Args, UWorld* World);
	void QueryItems(const TArray<FString>& Args, UWorld* World);

};

