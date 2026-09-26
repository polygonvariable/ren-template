// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "GameplayInstance.h"

// Project Headers
#include "GameLifecycleDelegate.h"


void UGameplayInstance::Init()
{
	Super::Init();
	GameInit();
}

void UGameplayInstance::GameInit()
{
	OnGameInstanceSubsystemsInitialized.Broadcast();
	FGameLifecycleDelegate::OnPreGameInitialized.Broadcast();
}

