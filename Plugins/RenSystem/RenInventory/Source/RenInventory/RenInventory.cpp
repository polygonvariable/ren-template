// Copyright Epic Games, Inc. All Rights Reserved.

#include "RenInventory.h"

#if WITH_EDITOR
#include "Public/InventorySubsystem.h"
#endif

#include "HAL/IConsoleManager.h"

#define LOCTEXT_NAMESPACE "FRenInventoryModule"



void FRenInventoryModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

#if WITH_EDITOR
	RegisterCommand();
#endif

}

void FRenInventoryModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

#if WITH_EDITOR
	UnregisterCommand();
#endif

}

#if WITH_EDITOR
void FRenInventoryModule::RegisterCommand()
{
	Commands.Add(IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("Inventory.AddItem"),
		TEXT("Add an item to an inventory container, args=<ContainerId> <ItemId> <Quantity>"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateRaw(this, &FRenInventoryModule::AddItem),
		ECVF_Cheat
	));
	Commands.Add(IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("Inventory.RemoveItem"),
		TEXT("Remove an item from an inventory container, args=<ContainerId> <ItemGuid> <Quantity>"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateRaw(this, &FRenInventoryModule::RemoveItem),
		ECVF_Cheat
	));
}

void FRenInventoryModule::UnregisterCommand()
{
	for (auto* Command : Commands)
	{
		IConsoleManager::Get().UnregisterConsoleObject(Command);
	}
}

void FRenInventoryModule::AddItem(const TArray<FString>& Args, UWorld* World)
{
	if (Args.Num() < 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddItem: Not enough arguments. Usage: Inventory.AddItem <ContainerId> <ItemId> <Quantity>"));
		return;
	}

	UGameInstance* GameInstance = World->GetGameInstance();
	if (IsValid(GameInstance))
	{
		UInventorySubsystem* InventorySubsystem = GameInstance->GetSubsystem<UInventorySubsystem>();
		if (IsValid(InventorySubsystem))
		{
			FName ContainerId = FName(*Args[0]);
			FName ItemId = FName(*Args[1]);
			int Quantity = FCString::Atoi(*Args[2]);

			//UInventoryAsset* InventoryAsset = InventorySubsystem->GetItemAsset(ItemId);
			//InventorySubsystem->AddItem(ContainerId, InventoryAsset, Quantity);
		}
	}
}

void FRenInventoryModule::RemoveItem(const TArray<FString>& Args, UWorld* World)
{
	if (Args.Num() < 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("RemoveItem: Not enough arguments. Usage: Inventory.RemoveItem <ContainerId> <ItemGuid> <Quantity>"));
		return;
	}

	UGameInstance* GameInstance = World->GetGameInstance();
	if (IsValid(GameInstance))
	{
		UInventorySubsystem* InventorySubsystem = GameInstance->GetSubsystem<UInventorySubsystem>();
		if (IsValid(InventorySubsystem))
		{
			FName ContainerId = FName(*Args[0]);
			FName ItemGuid = FName(*Args[1]);
			int Quantity = FCString::Atoi(*Args[2]);

			InventorySubsystem->RemoveItem(ContainerId, ItemGuid, Quantity);
		}
	}
}


#endif

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRenInventoryModule, RenInventory)

