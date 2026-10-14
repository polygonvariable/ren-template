// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "QuestDetailUI.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Engine/AssetManager.h"

// Project Headers
#include "Core/QuestPropertyDefinition.h"
#include "QuestEntryUI.h"
#include "QuestRuntimeDataUI.h"
#include "System/QuestSubsystem.h"


UQuestDetailUI::UQuestDetailUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TrackKey = EKeys::F;
}

void UQuestDetailUI::ShowDetail(UQuestEntry* Entry)
{
	if (!IsValid(Entry))
	{
		HideDetail();
		return;
	}

	ShowPrimaryDetail(Entry->QuestResult.QuestAssetId);
	ShowSecondayDetail(Entry);
}

void UQuestDetailUI::HideDetail()
{
	bQuestIsActive = false;
	QuestAssetId = FPrimaryAssetId();
	DetailSwitch->SetActiveWidgetIndex(0);
}

bool UQuestDetailUI::HandleOnKeyDown(const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == TrackKey)
	{
		HandleOnTrackClicked();
		return true;
	}
	if (QuestData->HandleOnKeyDown(InKeyEvent))
	{
		return true;
	}
	return false;
}

#if WITH_EDITOR
EDataValidationResult UQuestDetailUI::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);
	if (!TrackKey.IsValid())
	{
		Context.AddWarning(FText::FromString("Invalid track input key, input wont be handled"));
	}
	return Result;
}
#endif

void UQuestDetailUI::ShowPrimaryDetail(FPrimaryAssetId AssetId)
{
	if (!IsValid(AssetManager))
	{
		HideDetail();
		ensureMsgf(false, TEXT("Failed to get asset manager"));
		return;
	}

	FAssetData AssetData;
	if (!AssetManager->GetPrimaryAssetData(AssetId, AssetData))
	{
		HideDetail();
		ensureMsgf(false, TEXT("Failed to get asset data"));
		return;
	}

	FText Title = FText::GetEmpty();
	FText Description = FText::GetEmpty();

	FQuestAssetData::GetTitle(AssetData, Title);
	FQuestAssetData::GetDescription(AssetData, Description);

	QuestTitle->SetText(Title);
	QuestDescription->SetText(Description);
	DetailSwitch->SetActiveWidgetIndex(1);
}

void UQuestDetailUI::ShowSecondayDetail(UQuestEntry* Entry)
{
	check(Entry);

	QuestAssetId = Entry->QuestResult.QuestAssetId;
	bQuestIsActive = Entry->QuestResult.bActive;

	TrackButton->SetVisibility(Entry->QuestResult.bActive ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	QuestData->ShowDetail(Entry->QuestResult.QuestEngine.Get());
}

void UQuestDetailUI::HandleOnTrackClicked()
{
	if (IsValid(QuestSubsystem) && DetailSwitch->GetActiveWidgetIndex() == 1 && QuestAssetId.IsValid() && bQuestIsActive)
	{
		OnCloseRequest.ExecuteIfBound();
		QuestSubsystem->FocusQuest(QuestAssetId);
	}
}

void UQuestDetailUI::NativeConstruct()
{
	Super::NativeConstruct();

	AssetManager = UAssetManager::GetIfInitialized();
	QuestSubsystem = UQuestSubsystem::Get(GetWorld());

	TrackButton->OnClicked.AddDynamic(this, &UQuestDetailUI::HandleOnTrackClicked);
	QuestData->OnCloseRequest.BindWeakLambda(this, [this]() { OnCloseRequest.ExecuteIfBound(); });
}

void UQuestDetailUI::NativeDestruct()
{
	TrackButton->OnClicked.RemoveAll(this);
	QuestData->OnCloseRequest.Unbind();

	QuestSubsystem = nullptr;
	AssetManager = nullptr;

	Super::NativeDestruct();
}

