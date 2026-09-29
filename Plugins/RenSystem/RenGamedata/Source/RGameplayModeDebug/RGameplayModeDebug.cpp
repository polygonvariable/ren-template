// Copyright Epic Games, Inc. All Rights Reserved.

#include "RGameplayModeDebug.h"

#define LOCTEXT_NAMESPACE "FRGameplayModeDebugModule"

static bool LValue_RSeasonDebug = false;
static FAutoConsoleVariableRef CVarRSeasonDebug(
	TEXT("ren.GameplayMode.Debug"),
	LValue_RSeasonDebug,
	TEXT("Enable gameplay mode debug"),
	ECVF_Default
);

void FRGameplayModeDebugModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FRGameplayModeDebugModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRGameplayModeDebugModule, RGameplayModeDebug)

