// Copyright Epic Games, Inc. All Rights Reserved.

#include "RenInventory.h"

#define LOCTEXT_NAMESPACE "FRenInventoryModule"

void FRenInventoryModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FRenInventoryModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRenInventoryModule, RenInventory)

