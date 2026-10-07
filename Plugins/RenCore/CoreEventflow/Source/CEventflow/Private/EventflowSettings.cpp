// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowSettings.h"


UEventflowSettings::UEventflowSettings(const FObjectInitializer& ObjectInitializer)
{
	CategoryName = TEXT("Ren Project");
}

const UEventflowSettings* UEventflowSettings::Get()
{
	return GetDefault<UEventflowSettings>();
}

