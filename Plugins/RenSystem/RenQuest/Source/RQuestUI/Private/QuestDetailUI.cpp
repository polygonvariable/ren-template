// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "QuestDetailUI.h"

// Engine Headers
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Engine/AssetManager.h"

// Project Headers
#include "Core/QuestPropertyDefinition.h"
#include "QuestEntryUI.h"
#include "System/QuestSubsystem.h"
#include "System/Flow/QuestEngine.h"
#include "System/Flow/Task/QuestSubTask.h"
#include "System/Flow/Task/QuestPrimaryTask.h"
#include "EventflowAsset.h"
#include "Data/QuestAsset.h"


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

	TrackButton->SetVisibility(Entry->QuestResult.bActive ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	QuestData->ShowDetail(Entry->QuestResult.QuestEngine.Get(), QuestAssetId);
}

void UQuestDetailUI::HideDetail()
{
	QuestAssetId = FPrimaryAssetId();
	DetailSwitch->SetActiveWidgetIndex(0);
}

void UQuestDetailUI::HandleOnTrackClicked()
{
	if (IsValid(QuestSubsystem) && DetailSwitch->GetActiveWidgetIndex() == 1 && QuestAssetId.IsValid())
	{
		OnCloseRequest.ExecuteIfBound();
		QuestSubsystem->FocusQuest(QuestAssetId);
	}
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





UQuestRuntimeDataCollectionUI::UQuestRuntimeDataCollectionUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CancelKey = EKeys::P;
}

void UQuestRuntimeDataCollectionUI::ShowDetail(UQuestEngine* Engine, const FPrimaryAssetId& AssetId)
{
	HideDetail();
	
	if (IsValid(Engine))
	{
		ShowEngineDetail(Engine, AssetId);
		ShowGroupEngines(Engine);
	}
}

void UQuestRuntimeDataCollectionUI::HideDetail()
{
	QuestAssetId = FPrimaryAssetId();

	CancelButton->SetVisibility(ESlateVisibility::Collapsed);
	DataPanel->ClearChildren();
}

bool UQuestRuntimeDataCollectionUI::HandleOnKeyDown(const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == CancelKey)
	{
		HandleOnCancelClicked();
		return true;
	}
	return false;
}

void UQuestRuntimeDataCollectionUI::ShowEngineDetail(UQuestEngine* Engine, const FPrimaryAssetId& AssetId)
{
	check(IsValid(Engine));

	UQuestPrimaryTask* NodeTask = Engine->GetNodeTask<UQuestPrimaryTask>();
	if (IsValid(NodeTask))
	{
		ShowNodeDetail(NodeTask, AssetId);
		ShowSubDetail(NodeTask);
	}
}

void UQuestRuntimeDataCollectionUI::ShowNodeDetail(UQuestPrimaryTask* NodeTask, const FPrimaryAssetId& AssetId)
{
	check(IsValid(NodeTask));

	if (NodeTask->bShowInUI)
	{
		const FText& NodeTitle = NodeTask->ObjectiveTitle;
		const FText& NodeDescription = NodeTask->ObjectiveDescription;

		TInstancedStruct<FQuestRuntimeData> NodeRuntimeData;
		NodeTask->PullQuestData(NodeRuntimeData);

		const FQuestRuntimeData* NodeData = NodeRuntimeData.GetPtr();
		if (NodeData || !NodeTitle.IsEmpty() || !NodeDescription.IsEmpty())
		{
			UQuestRuntimeDataUI* NewItem = CreateWidget<UQuestRuntimeDataUI>(this, QuestDataClass);
			NewItem->ShowPrimaryDetail(NodeTitle, NodeDescription);
			NewItem->ShowRuntimeDetail(NodeData);

			DataPanel->AddChild(NewItem);
		}
	}

	if (!QuestAssetId.IsValid())
	{
		if (NodeTask->bCanBeCancelled)
		{
			QuestAssetId = AssetId;
			CancelButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			QuestAssetId = FPrimaryAssetId();
			CancelButton->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UQuestRuntimeDataCollectionUI::ShowSubDetail(UQuestPrimaryTask* NodeTask)
{
	check(IsValid(NodeTask));

	const TArray<TObjectPtr<UEventflowSubTask>>& SubTasks = NodeTask->GetSubTasks();
	for (UEventflowSubTask* Task : SubTasks)
	{
		UQuestSubTask* SubTask = Cast<UQuestSubTask>(Task);
		if (!IsValid(SubTask) || !SubTask->bShowInUI)
		{
			continue;
		}

		const FText& SubTitle = SubTask->ObjectiveTitle;
		const FText& SubDescription = SubTask->ObjectiveDescription;

		TInstancedStruct<FQuestRuntimeData> SubRuntimeData;
		SubTask->PullQuestData(SubRuntimeData);

		const FQuestRuntimeData* SubData = SubRuntimeData.GetPtr();
		if (SubData || !SubTitle.IsEmpty() || !SubDescription.IsEmpty())
		{
			UQuestRuntimeDataUI* NewItem = CreateWidget<UQuestRuntimeDataUI>(this, QuestDataClass);
			NewItem->ShowPrimaryDetail(SubTitle, SubDescription);
			NewItem->ShowRuntimeDetail(SubData);

			DataPanel->AddChild(NewItem);
		}
	}
}

void UQuestRuntimeDataCollectionUI::ShowGroupEngines(UQuestEngine* Engine)
{
	check(IsValid(Engine));

	const UEventflowAsset* Asset = Engine->GetAsset();
	if (!IsValid(Asset))
	{
		ensureMsgf(false, TEXT("Eventflow engine contains invalid asset"));
		return;
	}

	TMap<FPrimaryAssetId, TObjectPtr<UQuestEngine>> GroupedEngines;
	QuestSubsystem->GetQuestsByGroup(Asset->GetPrimaryAssetId(), GroupedEngines);

	for (const TPair<FPrimaryAssetId, TObjectPtr<UQuestEngine>>& Kv : GroupedEngines)
	{
		ShowEngineDetail(Kv.Value, Kv.Key);
	}
}

void UQuestRuntimeDataCollectionUI::HandleOnCancelClicked()
{
	if (IsValid(QuestSubsystem) && CancelButton->GetVisibility() == ESlateVisibility::Visible && QuestAssetId.IsValid())
	{
		OnCloseRequest.ExecuteIfBound();
		QuestSubsystem->CancelQuestTask(QuestAssetId, true);
	}
}





void UQuestRuntimeDataCollectionUI::NativeConstruct()
{
	Super::NativeConstruct();

	CancelButton->OnClicked.AddDynamic(this, &UQuestRuntimeDataCollectionUI::HandleOnCancelClicked);
	QuestSubsystem = UQuestSubsystem::Get(GetWorld());
}

void UQuestRuntimeDataCollectionUI::NativeDestruct()
{
	CancelButton->OnClicked.RemoveAll(this);
	QuestSubsystem = nullptr;

	Super::NativeDestruct();
}




void UQuestRuntimeDataUI::ShowPrimaryDetail(const FText& Title, const FText& Description)
{
	if (Title.IsEmpty())
	{
		TitleText->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		TitleText->SetText(Title);
		TitleText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (Description.IsEmpty())
	{
		DescriptionText->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		DescriptionText->SetText(Description);
		DescriptionText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UQuestRuntimeDataUI::ShowRuntimeDetail(const FQuestRuntimeData* Data)
{
	if (Data)
	{
		ValueText->SetText(FText::FromString(Data->Value));
		ValueText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		ValueText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

