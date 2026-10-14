// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "QuestDashboardUI.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Engine/AssetManager.h"

// Project Headers
#include "FilterUIInterface.h"
#include "QuestDetailUI.h"
#include "QuestEntryUI.h"
#include "System/QuestStorageManager.h"
#include "System/QuestSubsystem.h"


UQuestDashboardUI::UQuestDashboardUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CloseKey = EKeys::Escape;
	SetIsFocusable(true);
}

#if WITH_EDITOR
EDataValidationResult UQuestDashboardUI::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);
	if (!CloseKey.IsValid())
	{
		Context.AddWarning(FText::FromString("Invalid close input key, input wont be handled"));
	}
	if (!QuestQuery.FilterRule.CriterionRoot.IsValid())
	{
		Context.AddError(FText::FromString("Quest query contains invalid filter rule"));
		return EDataValidationResult::Invalid;
	}
	return Result;
}
#endif

void UQuestDashboardUI::HandleOnCloseClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UQuestDashboardUI::HandleOnVisibilityChanged(ESlateVisibility Visiblity)
{
	switch (GetVisibility())
	{
	case ESlateVisibility::Visible:
		HandleOnQuestChanged();
		break;
	case ESlateVisibility::Collapsed:
		QuestList->ClearListItems();
		QuestList->RegenerateAllEntries();
		break;
	}
}

void UQuestDashboardUI::HandleOnFilterSelected(const FInstancedStruct& Data)
{
	const FQuestQueryDefinition* Query = Data.GetPtr<FQuestQueryDefinition>();
	if (!Query)
	{
		ensureMsgf(false, TEXT("Quest query filter is invalid"));
		return;
	}
	QuestQuery = *Query;
	HandleOnQuestChanged();
}

void UQuestDashboardUI::HandleOnQuestChanged()
{
	if (!IsValid(QuestSubsystem) || !IsValid(AssetManager) || GetVisibility() != ESlateVisibility::Visible)
	{
		return;
	}

	QuestList->ClearListItems();
	QuestList->RegenerateAllEntries();
	
	if (IsValid(QuestSubsystem))
	{
		TArray<FQuestQueryResult> QueryResult;
		QuestSubsystem->QueryQuests(QuestQuery, QueryResult);

		for (const FQuestQueryResult& Result : QueryResult)
		{
			UQuestEntry* Entry = NewObject<UQuestEntry>(this);
			if (!IsValid(Entry))
			{
				continue;
			}
			Entry->QuestResult = Result;
			QuestList->AddItem(Entry);
		}
	}
}

void UQuestDashboardUI::HandleOnQuestSelectionChanged(UObject* Object)
{
	QuestDetail->ShowDetail(Cast<UQuestEntry>(Object));
}

void UQuestDashboardUI::NativeConstruct()
{
	Super::NativeConstruct();

	AssetManager = UAssetManager::GetIfInitialized();

	OnVisibilityChanged.AddDynamic(this, &UQuestDashboardUI::HandleOnVisibilityChanged);

	CloseButton->OnClicked.AddDynamic(this, &UQuestDashboardUI::HandleOnCloseClicked);
	QuestList->OnItemSelectionChanged().AddUObject(this, &UQuestDashboardUI::HandleOnQuestSelectionChanged);
	QuestDetail->OnCloseRequest.BindUObject(this, &UQuestDashboardUI::HandleOnCloseClicked);

	IFilterUIInterface* FilterInterface = Cast<IFilterUIInterface>(FilterCollection);
	if (FilterInterface)
	{
		FilterInterface->GetOnFilterSelected().BindUObject(this, &UQuestDashboardUI::HandleOnFilterSelected);
	}

	QuestSubsystem = UQuestSubsystem::Get(GetWorld());
	if (IsValid(QuestSubsystem))
	{
		HandleOnQuestChanged();
		QuestSubsystem->GetOnEngineAdded().AddWeakLambda(this, [this](FPrimaryAssetId, UEventflowEngine*) { HandleOnQuestChanged(); });
		QuestSubsystem->GetOnEngineRemoved().AddWeakLambda(this, [this](FPrimaryAssetId, UEventflowEngine*) { HandleOnQuestChanged(); });
	}
}

void UQuestDashboardUI::NativeDestruct()
{
	AssetManager = nullptr;

	OnVisibilityChanged.RemoveAll(this);

	CloseButton->OnClicked.RemoveAll(this);
	QuestList->OnItemSelectionChanged().RemoveAll(this);
	QuestDetail->OnCloseRequest.Unbind();

	IFilterUIInterface* FilterInterface = Cast<IFilterUIInterface>(FilterCollection);
	if (FilterInterface)
	{
		FilterInterface->GetOnFilterSelected().Unbind();
	}

	if (IsValid(QuestSubsystem))
	{
		QuestSubsystem->GetOnEngineAdded().RemoveAll(this);
		QuestSubsystem->GetOnEngineRemoved().RemoveAll(this);
	}
	QuestSubsystem = nullptr;
	
	Super::NativeDestruct();
}

FReply UQuestDashboardUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == CloseKey)
	{
		HandleOnCloseClicked();
		return FReply::Handled();
	}
	if (QuestDetail->HandleOnKeyDown(InKeyEvent))
	{
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

