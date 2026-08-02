// Copyright Epic Games, Inc. All Rights Reserved.

#include "RSettings.h"
#include "HAL/IConsoleManager.h"

#define LOCTEXT_NAMESPACE "FRSettingsModule"

static TAutoConsoleVariable<float> CVarSFXVolume(TEXT("ren.audio.SFXVolume"), 1.0f, TEXT("Controls the SFX Volume for the RPG Audio Plugin.\n") TEXT("0.0 = silent, 1.0 = full volume"), ECVF_SetByGameSetting);

void FRSettingsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FRSettingsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRSettingsModule, RSettings)

