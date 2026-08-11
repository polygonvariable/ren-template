// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarDetailUI.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Data/AvatarAsset.h"
#include "Core/Type/Runtime/AvatarInstance.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "System/AvatarStorageManager.h"
#include "System/AvatarSubsystem.h"
#include "Widget/AscensionDetailUI.h"
#include "Widget/AvatarEntry.h"


void UAvatarDetailUI::InitializeDetail()
{
	UAvatarSubsystem* AvatarSubsystem = UAvatarSubsystem::Get(GetGameInstance());
	if (!IsValid(AvatarSubsystem))
	{
		LOG_ERROR(LogAvatar, TEXT("AvatarSubsystem is invalid"));
		return;
	}

	StorageManager = AvatarSubsystem->GetStorageManager();
	if (IsValid(StorageManager) && bAutoRefresh)
	{
		StorageManager->OnStorageUpdated.AddUObject(this, &UAvatarDetailUI::RefreshDetail);
	}
}

void UAvatarDetailUI::RefreshDetail()
{
	if (!IsValid(StorageManager))
	{
		return;
	}

	const FAvatarInstance* AvatarInstance = StorageManager->GetInstance(GetActiveAssetId());
	if (!AvatarInstance)
	{
		return;
	}

	SetCustomDetails(AvatarInstance);
}

void UAvatarDetailUI::SetPrimaryDetail(const UCoreDataAsset* Asset)
{
	const UAvatarAsset* AvatarAsset = Cast<UAvatarAsset>(Asset);
	if (!IsValid(AvatarAsset))
	{
		SwitchDetail(false);
		return;
	}

	SwitchDetail(true);

	EntryName->SetText(AvatarAsset->DisplayName);
	EntryDescription->SetText(AvatarAsset->Description);
	EntryIcon->SetBrushFromSoftTexture(AvatarAsset->Icon);
}

void UAvatarDetailUI::SetSecondaryDetail(const UAssetEntry* Entry)
{
	const UAvatarEntry* AvatarEntry = Cast<UAvatarEntry>(Entry);
	if (!IsValid(AvatarEntry))
	{
		return;
	}

	const FAvatarInstance* Instance = AvatarEntry->AvatarInstance;
	SetCustomDetails(Instance);
}

void UAvatarDetailUI::SetCustomDetails(const FAvatarInstance* Instance)
{
	if (!Instance)
	{
		return;
	}

	AscensionDetail->InitializeDetail(Instance->Ascension);
}

void UAvatarDetailUI::NativeDestruct()
{
	if (IsValid(StorageManager))
	{
		StorageManager->OnStorageUpdated.RemoveAll(this);
	}
	StorageManager = nullptr;

	Super::NativeDestruct();
}

