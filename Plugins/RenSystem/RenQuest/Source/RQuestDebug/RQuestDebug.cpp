// Copyright Epic Games, Inc. All Rights Reserved.

#include "RQuestDebug.h"

#define LOCTEXT_NAMESPACE "FRQuestDebugModule"

static bool LValue_RQuestDebug = false;
static FAutoConsoleVariableRef CVarRQuestDebug(
	TEXT("ren.Quest.Debug"),
	LValue_RQuestDebug,
	TEXT("Enable quest debug"),
	ECVF_Default
);

void FRQuestDebugModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FRQuestDebugModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRQuestDebugModule, RQuestDebug)

