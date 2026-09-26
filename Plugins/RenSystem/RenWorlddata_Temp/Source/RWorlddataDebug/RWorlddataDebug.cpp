// Copyright Epic Games, Inc. All Rights Reserved.

#include "RWorlddataDebug.h"

#define LOCTEXT_NAMESPACE "FRWorlddataDebugModule"

static bool LValue_RGamedataClockDebug = false;
static FAutoConsoleVariableRef CVarRWorlddataDebug(
	TEXT("ren.Gamedata.Clock.Debug"),
	LValue_RGamedataClockDebug,
	TEXT("Enable clock debug"),
	ECVF_Default
);

void FRWorlddataDebugModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FRWorlddataDebugModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRWorlddataDebugModule, RWorlddataDebug)

