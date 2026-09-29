// Copyright Epic Games, Inc. All Rights Reserved.

#include "RDialogueDebug.h"

#define LOCTEXT_NAMESPACE "FRDialogueDebugModule"

static bool LValue_RDialogueDebug = false;
static FAutoConsoleVariableRef CVarRDialogueDebug(
	TEXT("ren.Dialogue.Debug"),
	LValue_RDialogueDebug,
	TEXT("Enable dialogue debug"),
	ECVF_Default
);

void FRDialogueDebugModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FRDialogueDebugModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRDialogueDebugModule, RDialogueDebug)

