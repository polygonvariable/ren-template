// Copyright Epic Games, Inc. All Rights Reserved.

#include "REnvironmentPaint.h"

#define LOCTEXT_NAMESPACE "FREnvironmentPaintModule"

static bool LValue_REnvironmentPaint = false;
static FAutoConsoleVariableRef CVarREnvironmentPaint(
	TEXT("ren.Environment.Paint"),
	LValue_REnvironmentPaint,
	TEXT("Enables environment painting"),
	ECVF_Default
);

#if WITH_EDITOR
static bool LValue_REnvironmentPaintDebug = false;
static FAutoConsoleVariableRef CVarREnvironmentPaintDebug(
	TEXT("ren.Environment.Paint.Debug"),
	LValue_REnvironmentPaintDebug,
	TEXT("Enables environment painting debug"),
	ECVF_Default
);
#endif

void FREnvironmentPaintModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FREnvironmentPaintModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FREnvironmentPaintModule, REnvironmentPaint)

