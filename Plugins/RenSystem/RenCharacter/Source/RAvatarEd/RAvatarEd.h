// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

// Forward declaration
class UAvatarStorage;



class FRAvatarEdModule : public IModuleInterface
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

	void AddItem(const TArray<FString>& Args, UWorld* World);
	void RemoveItem(const TArray<FString>& Args, UWorld* World);
	void GetItem(const TArray<FString>& Args, UWorld* World);

	UAvatarStorage* GetAvatarStorage(UWorld* World);

};

