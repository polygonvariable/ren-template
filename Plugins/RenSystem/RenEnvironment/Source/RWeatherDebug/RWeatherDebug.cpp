// Copyright Epic Games, Inc. All Rights Reserved.

#include "RWeatherDebug.h"

#define LOCTEXT_NAMESPACE "FRWeatherDebugModule"

static bool LValue_RWeatherDebug = false;
static FAutoConsoleVariableRef CVarRWeatherDebug(
	TEXT("ren.Environment.Weather.Debug"),
	LValue_RWeatherDebug,
	TEXT("Enable weather debug"),
	ECVF_Default
);

void FRWeatherDebugModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FRWeatherDebugModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRWeatherDebugModule, RWeatherDebug)

