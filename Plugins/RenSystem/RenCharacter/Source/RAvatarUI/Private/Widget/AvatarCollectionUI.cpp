// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarCollectionUI.h"

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Storage/AvatarStorageManager.h"
#include "Subsystem/AvatarSubsystem.h"
#include "Widget/AvatarEntry.h"


void UAvatarCollectionUI::InitializeCollection()
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
		StorageManager->OnStorageUpdated.AddUObject(this, &UAvatarCollectionUI::RefreshEntries);
	}
}

void UAvatarCollectionUI::DisplayEntries()
{
	if (!IsValid(StorageManager))
	{
		LOG_ERROR(LogAvatar, TEXT("AvatarStorage is invalid"));
		return;
	}

	TArray<FAvatarSortEntry> SortedEntries;
	StorageManager->QueryInstances(GetFilterRoot(), QueryRule, SortedEntries);

	for (const FAvatarSortEntry& SortEntry : SortedEntries)
	{
		UAvatarEntry* Entry = GetEntryFromPool<UAvatarEntry>();
		if (IsValid(Entry))
		{
			Entry->AvatarInstance = SortEntry.Instance;
			AddEntry(SortEntry.AssetId, Entry);
		}
	}
}

void UAvatarCollectionUI::NativeDestruct()
{
	if (IsValid(StorageManager))
	{
		StorageManager->OnStorageUpdated.RemoveAll(this);
	}
	StorageManager = nullptr;

	Super::NativeDestruct();
}

