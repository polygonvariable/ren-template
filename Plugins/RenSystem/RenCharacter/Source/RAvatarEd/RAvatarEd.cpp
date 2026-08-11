// Copyright Epic Games, Inc. All Rights Reserved.

// Parent Header
#include "RAvatarEd.h"

// Engine Headers
#include "HAL/IConsoleManager.h"

// Project Headers
#include "Core/CharacterPrimaryAsset.h"
#include "System/AvatarSubsystem.h"
#include "System/AvatarStorageManager.h"
#include "Core/AvatarSettings.h"

#define LOCTEXT_NAMESPACE "FRAvatarEdModule"



void FRAvatarEdModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	RegisterCommand();
}

void FRAvatarEdModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UnregisterCommand();
}

void FRAvatarEdModule::RegisterCommand()
{
	IConsoleManager& ConsoleManager = IConsoleManager::Get();

	Commands.Add(ConsoleManager.RegisterConsoleCommand(
		TEXT("Avatar.AddItem"),
		TEXT("Add avatar, args=<AssetId>"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateRaw(this, &FRAvatarEdModule::AddItem),
		ECVF_Cheat
	));
	Commands.Add(ConsoleManager.RegisterConsoleCommand(
		TEXT("Avatar.RemoveItem"),
		TEXT("Remove avatar, args=<AssetId>"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateRaw(this, &FRAvatarEdModule::RemoveItem),
		ECVF_Cheat
	));
	Commands.Add(ConsoleManager.RegisterConsoleCommand(
		TEXT("Avatar.GetItem"),
		TEXT("Prints details of avatar, args=<AssetId>"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateRaw(this, &FRAvatarEdModule::GetItem),
		ECVF_Cheat
	));
}

void FRAvatarEdModule::UnregisterCommand()
{
	IConsoleManager& ConsoleManager = IConsoleManager::Get();

	for (IConsoleCommand* Command : Commands)
	{
		ConsoleManager.UnregisterConsoleObject(Command);
	}
}

UAvatarStorageManager* FRAvatarEdModule::GetStorageManager(UWorld* World)
{
	UAvatarSubsystem* Subsystem = UAvatarSubsystem::Get(World);
	if (!IsValid(Subsystem))
	{
		return nullptr;
	}
	return Subsystem->GetStorageManager();
}

void FRAvatarEdModule::AddItem(const TArray<FString>& Args, UWorld* World)
{
	if (Args.Num() < 1 || !IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid arguments"));
		return;
	}

	UAvatarStorageManager* Collection = GetStorageManager(World);
	if (IsValid(Collection))
	{
		FPrimaryAssetId AssetId = FCharacterPrimaryAsset::GetPrimaryAssetId(FName(*Args[0]));
		Collection->AddInstance(AssetId, 1);
	}
}

void FRAvatarEdModule::RemoveItem(const TArray<FString>& Args, UWorld* World)
{
	if (Args.Num() < 1 || !IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid arguments"));
		return;
	}

	UAvatarStorageManager* Collection = GetStorageManager(World);
	if (IsValid(Collection))
	{
		FPrimaryAssetId AssetId = FCharacterPrimaryAsset::GetPrimaryAssetId(FName(*Args[0]));
		Collection->RemoveInstance(AssetId, 1);
	}
}

void FRAvatarEdModule::GetItem(const TArray<FString>& Args, UWorld* World)
{
	if (Args.Num() < 1 || !IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid arguments"));
		return;
	}

	UAvatarStorageManager* Collection = GetStorageManager(World);
	if (IsValid(Collection))
	{
		FPrimaryAssetId AssetId = FCharacterPrimaryAsset::GetPrimaryAssetId(FName(*Args[0]));
		const FAvatarInstance* Item = Collection->GetInstance(AssetId);
		if (!Item)
		{
			UE_LOG(LogTemp, Error, TEXT("Avatar not found"));
			return;
		}

		FString ItemDebug = Item->ToString();
		UE_LOG(LogTemp, Log, TEXT("%s"), *ItemDebug);
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRAvatarEdModule, RAvatarEd)

