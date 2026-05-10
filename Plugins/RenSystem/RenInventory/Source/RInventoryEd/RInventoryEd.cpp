// Copyright Epic Games, Inc. All Rights Reserved.

// Parent Header
#include "RInventoryEd.h"

// Engine Headers
#include "HAL/IConsoleManager.h"

// Project Headers
#include "Definition/Runtime/InventoryInstance.h"
#include "Library/InventoryPrimaryAsset.h"
#include "Storage/InventoryStorageManager.h"
#include "Subsystem/InventorySubsystem.h"

#define LOCTEXT_NAMESPACE "FRInventoryEdModule"



void FRInventoryEdModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	RegisterCommand();
}

void FRInventoryEdModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UnregisterCommand();
}

void FRInventoryEdModule::RegisterCommand()
{
	IConsoleManager& ConsoleManager = IConsoleManager::Get();

	Commands.Add(ConsoleManager.RegisterConsoleCommand(
		TEXT("Inventory.AddItem"),
		TEXT("Add an item to inventory, args=<CollectionId> <AssetId> <Quantity>"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateRaw(this, &FRInventoryEdModule::AddItem),
		ECVF_Cheat
	));
	Commands.Add(ConsoleManager.RegisterConsoleCommand(
		TEXT("Inventory.RemoveItem"),
		TEXT("Remove an item from inventory, args=<CollectionId> <AssetId> <Quantity>"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateRaw(this, &FRInventoryEdModule::RemoveItem),
		ECVF_Cheat
	));
	Commands.Add(ConsoleManager.RegisterConsoleCommand(
		TEXT("Inventory.GetItem"),
		TEXT("Prints details of an item, args=<CollectionId> <AssetId>"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateRaw(this, &FRInventoryEdModule::GetItem),
		ECVF_Cheat
	));
	Commands.Add(ConsoleManager.RegisterConsoleCommand(
		TEXT("Inventory.QueryItems"),
		TEXT("Prints all items, args=<CollectionId>"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateRaw(this, &FRInventoryEdModule::QueryItems),
		ECVF_Cheat
	));
}

void FRInventoryEdModule::UnregisterCommand()
{
	IConsoleManager& ConsoleManager = IConsoleManager::Get();

	for (IConsoleCommand* Command : Commands)
	{
		ConsoleManager.UnregisterConsoleObject(Command);
	}
}

UInventoryStorageManager* FRInventoryEdModule::GetStorageManager(UWorld* World, FName ContainerId)
{
	UInventorySubsystem* InventorySubsystem = UInventorySubsystem::Get(World);
	if (!IsValid(InventorySubsystem))
	{
		return nullptr;
	}
	return InventorySubsystem->GetStorageManager(ContainerId);
}

void FRInventoryEdModule::AddItem(const TArray<FString>& Args, UWorld* World)
{
	if (Args.Num() < 3 || !IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid arguments"));
		return;
	}

	UInventoryStorageManager* StorageManager = GetStorageManager(World, FName(*Args[0]));
	if (IsValid(StorageManager))
	{
		FPrimaryAssetId AssetId = FInventoryPrimaryAsset::GetPrimaryAssetId(FName(*Args[1]));
		int Quantity = FCString::Atoi(*Args[2]);

		StorageManager->AddInstance(AssetId, Quantity);
	}
}

void FRInventoryEdModule::RemoveItem(const TArray<FString>& Args, UWorld* World)
{
	if (Args.Num() < 3 || !IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid arguments"));
		return;
	}

	UInventoryStorageManager* StorageManager = GetStorageManager(World, FName(*Args[0]));
	if (IsValid(StorageManager))
	{
		FPrimaryAssetId AssetId = FInventoryPrimaryAsset::GetPrimaryAssetId(FName(*Args[1]));
		int Quantity = FCString::Atoi(*Args[2]);

		StorageManager->RemoveInstance(AssetId, Quantity);
	}
}

void FRInventoryEdModule::GetItem(const TArray<FString>& Args, UWorld* World)
{
	if (Args.Num() < 2 || !IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid arguments"));
		return;
	}

	UInventoryStorageManager* StorageManager = GetStorageManager(World, FName(*Args[0]));
	if (IsValid(StorageManager))
	{
		FPrimaryAssetId AssetId = FInventoryPrimaryAsset::GetPrimaryAssetId(FName(*Args[1]));
		const FInventoryInstance* Item = StorageManager->GetInstance(AssetId);
		if (!Item)
		{
			UE_LOG(LogTemp, Error, TEXT("Item not found"));
			return;
		}

		FString ItemDebug = Item->ToString();
		UE_LOG(LogTemp, Log, TEXT("%s"), *ItemDebug);
	}
}

void FRInventoryEdModule::QueryItems(const TArray<FString>& Args, UWorld* World)
{
	if (Args.Num() < 1 || !IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid arguments"));
		return;
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRInventoryEdModule, RInventoryEd)

