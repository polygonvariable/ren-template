// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "RenGameInstance.h"

// Project Headers
#include "RenCore/Public/Delegate/LatentDelegates.h"



void URenGameInstance::Init()
{
	Super::Init();
	FLatentDelegates::OnGameInstanceSubsystemsInitialized.Broadcast();
}

