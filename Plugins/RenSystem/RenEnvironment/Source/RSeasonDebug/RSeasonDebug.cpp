// Copyright Epic Games, Inc. All Rights Reserved.

#include "RSeasonDebug.h"

#define LOCTEXT_NAMESPACE "FRSeasonDebugModule"

static bool LValue_RSeasonDebug = false;
static FAutoConsoleVariableRef CVarRSeasonDebug(
	TEXT("ren.Environment.Season.Debug"),
	LValue_RSeasonDebug,
	TEXT("Enable season debug"),
	ECVF_Default
);

void FRSeasonDebugModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FRSeasonDebugModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRSeasonDebugModule, RSeasonDebug)

