// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "QuestEntryUI.h"

// Engine Headers
#include "Components/TextBlock.h"
#include "Engine/AssetManager.h"

// Project Headers
#include "Core/QuestPropertyDefinition.h"


void UQuestEntryUI::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	const UQuestEntry* Entry = CastChecked<UQuestEntry>(ListItemObject);

	if (!IsValid(AssetManager))
	{
		ensureMsgf(false, TEXT("Failed to get asset manager"));
		return;
	}

	FAssetData AssetData;
	if (!AssetManager->GetPrimaryAssetData(Entry->QuestResult.QuestAssetId, AssetData))
	{
		ensureMsgf(false, TEXT("Failed to get asset data"));
		return;
	}

	FText Title = FText::GetEmpty();

	FQuestAssetData::GetTitle(AssetData, Title);

	QuestTitle->SetText(Title);
}

void UQuestEntryUI::NativeConstruct()
{
	Super::NativeConstruct();
	AssetManager = UAssetManager::GetIfInitialized();
}

void UQuestEntryUI::NativeDestruct()
{
	AssetManager = nullptr;
	Super::NativeDestruct();
}

