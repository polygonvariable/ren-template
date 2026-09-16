// Copyright Epic Games, Inc. All Rights Reserved.

#include "RWorldDataDebug.h"

#define LOCTEXT_NAMESPACE "FRWorldDataDebugModule"

static bool LValue_RGamedataClockDebug = false;
static FAutoConsoleVariableRef CVarRWorldDataDebug(
	TEXT("ren.Gamedata.Clock.Debug"),
	LValue_RGamedataClockDebug,
	TEXT("Enable clock debug"),
	ECVF_Default
);

void FRWorldDataDebugModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FRWorldDataDebugModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRWorldDataDebugModule, RWorldDataDebug)

