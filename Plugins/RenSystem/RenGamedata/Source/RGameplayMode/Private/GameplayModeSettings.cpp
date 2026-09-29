// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "GameplayModeSettings.h"


UGameplayModeSettings::UGameplayModeSettings(const FObjectInitializer& ObjectInitializer)
{
	CategoryName = TEXT("Ren Project");
}

const UGameplayModeSettings* UGameplayModeSettings::Get()
{
	return GetDefault<UGameplayModeSettings>();
}

TArray<FName> UGameplayModeSettings::GetGameplayModeTableRows()
{
	const UGameplayModeSettings* Settings = UGameplayModeSettings::Get();
	const UDataTable* Table = Settings->GameplayModeTable.LoadSynchronous();
	if (!IsValid(Table))
	{
		return TArray<FName>();
	}
	return Table->GetRowNames();
}

