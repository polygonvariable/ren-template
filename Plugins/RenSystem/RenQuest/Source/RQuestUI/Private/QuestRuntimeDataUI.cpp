// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "QuestRuntimeDataUI.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "Components/Button.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Core/PoolLibrary.h"
#include "Data/QuestAsset.h"
#include "EventflowAsset.h"
#include "System/Flow/QuestEngine.h"
#include "System/Flow/Task/QuestPrimaryTask.h"
#include "System/Flow/Task/QuestSubTask.h"
#include "System/QuestSubsystem.h"


void UQuestRuntimeDataUI::SetPrimaryDetail(const FText& Title, const FText& Description)
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

void UQuestRuntimeDataUI::SetRuntimeDetail(const FQuestRuntimeData* Data)
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


UQuestRuntimeDataCollectionUI::UQuestRuntimeDataCollectionUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CancelKey = EKeys::P;
}

void UQuestRuntimeDataCollectionUI::CancelObjective()
{
	HandleOnCancelClicked();
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


void UQuestRuntimeDataCollectionUI::ShowDetail(UQuestEngine* Engine)
{
	HideDetail();
	
	if (IsValid(Engine))
	{
		const UQuestAsset* Asset = Engine->GetAsset<UQuestAsset>();
		if (!IsValid(Asset))
		{
			ensureMsgf(false, TEXT("Quest engine contains invalid asset"));
			return;
		}

		ShowEngineDetail(Engine, Asset->GetPrimaryAssetId());
		ShowGroupEngines(Engine);
	}
}

void UQuestRuntimeDataCollectionUI::HideDetail()
{
	QuestAssetId = FPrimaryAssetId();

	CancelButton->SetVisibility(ESlateVisibility::Collapsed);

	TArray<UWidget*> Widgets = DataPanel->GetAllChildren();
	for (UWidget* Widget : Widgets)
	{
		UQuestRuntimeDataUI* DataWidget = Cast<UQuestRuntimeDataUI>(Widget);
		if (IsValid(DataWidget))
		{
			FPoolLibrary::ReturnToArray(QuestDataPool, DataWidget);
		}
	}
	DataPanel->ClearChildren();
}


#if WITH_EDITOR
EDataValidationResult UQuestRuntimeDataCollectionUI::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (!CancelKey.IsValid())
	{
		Context.AddWarning(FText::FromString("Invalid cancel input key, input wont be handled"));
	}

	if (!IsValid(QuestDataClass))
	{
		Context.AddError(FText::FromString("Invalid quest data widget class"));
		return EDataValidationResult::Invalid;
	}

	return Result;
}
#endif


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

		RenderDetail(NodeTitle, NodeDescription, NodeRuntimeData.GetPtr());
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

		RenderDetail(SubTitle, SubDescription, SubRuntimeData.GetPtr());
	}
}

void UQuestRuntimeDataCollectionUI::RenderDetail(const FText& Title, const FText& Description, const FQuestRuntimeData* Data)
{
	check(QuestDataClass);

	if (Data || !Title.IsEmpty() || !Description.IsEmpty())
	{
		UQuestRuntimeDataUI* NewItem = FPoolLibrary::AcquireFromArray<UQuestRuntimeDataUI>(QuestDataPool, QuestDataClass, this);
		if (!IsValid(NewItem))
		{
			return;
		}

		DataPanel->AddChild(NewItem);

		NewItem->SetPrimaryDetail(Title, Description);
		NewItem->SetRuntimeDetail(Data);
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

