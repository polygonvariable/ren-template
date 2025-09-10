// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "RenGameInstance.h"

// Project Headers
#include "RCoreDelegate/Public/LatentDelegates.h"



void URenGameInstance::Init()
{
	Super::Init();
	FLatentDelegates::OnPreGameInitialized.Broadcast();
}

