// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "QuestUI.h"

// Engine Headers
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "EnhancedInputComponent.h"

// Project Headers
#include "Data/QuestAsset.h"
#include "EventflowTask.h"
#include "System/Flow/QuestEngine.h"
#include "System/Flow/Task/QuestPrimaryTask.h"
#include "System/Flow/Task/QuestSubTask.h"
#include "System/QuestSubsystem.h"


void UQuestItemUI::FocusQuest()
{
	if (AssetId.IsValid())
	{
		UQuestSubsystem::Get(GetWorld())->FocusQuest(AssetId);
	}
}

void UQuestItemUI::CancelQuest()
{
}



void UQuestUI::RenderSubTask(UEventflowNodeTask* NodeTask, UQuestItemUI* NewItem)
{
	FString SubtaskData;

	const TArray<TObjectPtr<UEventflowSubTask>>& Subtasks = NodeTask->GetSubTasks();
	for (UEventflowSubTask* SubTask : Subtasks)
	{
		UQuestSubTask* QSub = Cast<UQuestSubTask>(SubTask);
		if (QSub)
		{
			TInstancedStruct<FQuestRuntimeData> QData;
			QSub->PullQuestData(QData);

			SubtaskData += QSub->TaskName.ToString();

			FQuestRuntimeData* QDataPtr = QData.GetMutablePtr();
			if (QDataPtr)
			{
				SubtaskData += " : " + QDataPtr->Value;
			}

			SubtaskData += "\n";
		}
	}

	NewItem->SubTaskTextBlock->SetText(FText::FromString(SubtaskData.TrimStartAndEnd()));
}

void UQuestUI::RenderNodeTask(UQuestEngine* Engine, UEventflowNodeTask* NodeTask, UQuestItemUI* NewItem)
{
	if (NodeTask)
	{
		UQuestPrimaryTask* QTask = Cast<UQuestPrimaryTask>(NodeTask);
		if (QTask->bCanCancel)
		{
			NewItem->CancelButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			NewItem->CancelButton->SetVisibility(ESlateVisibility::Collapsed);
		}

		NewItem->NodeTaskTextBlock->SetText(FText::FromName(NodeTask->TaskName));

		RenderSubTask(NodeTask, NewItem);

		TMap<FPrimaryAssetId, TObjectPtr<UQuestEngine>> GroupEngines;
		QuestSubsystem->GetQuestsByGroup(Engine->GetAsset()->GetPrimaryAssetId(), GroupEngines);

		RenderGroupQuests(GroupEngines, NewItem);
	}
}

void UQuestUI::RenderMainQuests(const TMap<FPrimaryAssetId, TObjectPtr<UQuestEngine>>& Quests)
{
	for (const TPair<FPrimaryAssetId, TObjectPtr<UQuestEngine>>& Kv : Quests)
	{
		UQuestEngine* Engine = Kv.Value.Get();
		if (!Engine) continue;

		const FEventflowEntryData* Data = Engine->GetEntryData().GetPtr<FEventflowEntryData>();
		if (!Data) continue;

		if (Data->GroupId.IsValid())
		{
			continue;
		}

		UQuestAsset* Asset = Engine->GetAsset<UQuestAsset>();

		UQuestItemUI* NewItem = CreateWidget<UQuestItemUI>(this, ItemClass);
		NewItem->TitleTextBlock->SetText(Asset->Title);
		NewItem->AssetId = Asset->GetPrimaryAssetId();

		UEventflowNodeTask* NodeTask = Engine->GetNodeTask();
		if (!NodeTask) return;

		RenderNodeTask(Engine, NodeTask, NewItem);

		QuestPanel->AddChild(NewItem);
	}
}


void UQuestUI::RenderGroupSubTask(UEventflowNodeTask* NodeTask, UQuestItemUI* GroupItem)
{
	FString SubtaskData;

	const TArray<TObjectPtr<UEventflowSubTask>>& Subtasks = NodeTask->GetSubTasks();
	for (UEventflowSubTask* SubTask : Subtasks)
	{
		UQuestSubTask* QSub = Cast<UQuestSubTask>(SubTask);
		if (QSub)
		{
			TInstancedStruct<FQuestRuntimeData> QData;
			QSub->PullQuestData(QData);

			SubtaskData += QSub->TaskName.ToString();

			FQuestRuntimeData* QDataPtr = QData.GetMutablePtr();
			if (QDataPtr)
			{
				SubtaskData += " : " + QDataPtr->Value;
			}

			SubtaskData += "\n";
		}
	}

	GroupItem->SubTaskTextBlock->SetText(FText::FromString(SubtaskData.TrimStartAndEnd()));
}

void UQuestUI::RenderGroupNodeTask(UQuestEngine* Engine, UEventflowNodeTask* NodeTask, UQuestItemUI* GroupItem)
{
	if (NodeTask)
	{
		UQuestPrimaryTask* QTask = Cast<UQuestPrimaryTask>(NodeTask);
		if (QTask->bCanCancel)
		{
			GroupItem->CancelButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			GroupItem->CancelButton->SetVisibility(ESlateVisibility::Collapsed);
		}

		GroupItem->NodeTaskTextBlock->SetText(FText::FromName(NodeTask->TaskName));
		RenderGroupSubTask(NodeTask, GroupItem);
	}
}

void UQuestUI::RenderGroupQuests(const TMap<FPrimaryAssetId, TObjectPtr<UQuestEngine>>& GroupEngines, UQuestItemUI* NewItem)
{
	for (const TPair<FPrimaryAssetId, TObjectPtr<UQuestEngine>>& Kv : GroupEngines)
	{
		UQuestEngine* Engine = Kv.Value.Get();
		if (!Engine) continue;

		UQuestAsset* Asset = Engine->GetAsset<UQuestAsset>();

		UQuestItemUI* GroupItem = CreateWidget<UQuestItemUI>(this, ItemClass);
		GroupItem->TitleTextBlock->SetText(Asset->Title);
		GroupItem->AssetId = Asset->GetPrimaryAssetId();

		UEventflowNodeTask* NodeTask = Engine->GetNodeTask();
		if (!NodeTask) continue;

		RenderGroupNodeTask(Engine, NodeTask, GroupItem);

		NewItem->GroupPanel->AddChild(GroupItem);
	}
}


void UQuestUI::HandleOnQuestUpdated()
{
	QuestPanel->ClearChildren();

	const TMap<FPrimaryAssetId, TObjectPtr<UQuestEngine>>& Quests = QuestSubsystem->GetQuests();
	RenderMainQuests(Quests);
}

void UQuestUI::NativeConstruct()
{
	Super::NativeConstruct();

	QuestSubsystem = UQuestSubsystem::Get(GetWorld());
	if (IsValid(QuestSubsystem))
	{
		HandleOnQuestUpdated();
		QuestSubsystem->OnQuestDataChanged.AddWeakLambda(this, [this]() { this->HandleOnQuestUpdated(); });
		QuestSubsystem->GetOnEngineAdded().AddWeakLambda(this, [this](FPrimaryAssetId, UEventflowEngine*) { this->HandleOnQuestUpdated(); });
		QuestSubsystem->GetOnEngineRemoved().AddWeakLambda(this, [this](FPrimaryAssetId, UEventflowEngine*) { this->HandleOnQuestUpdated(); });
	}
}

void UQuestUI::NativeDestruct()
{
	QuestSubsystem = UQuestSubsystem::Get(GetWorld());
	if (IsValid(QuestSubsystem))
	{
		QuestSubsystem->OnQuestDataChanged.RemoveAll(this);
		QuestSubsystem->GetOnEngineAdded().RemoveAll(this);
		QuestSubsystem->GetOnEngineRemoved().RemoveAll(this);
	}

	Super::NativeDestruct();
}



void UQuestFocusUI::RenderSubTask(UEventflowNodeTask* NodeTask, UQuestItemUI* NewItem)
{
	FString SubtaskData;

	const TArray<TObjectPtr<UEventflowSubTask>>& Subtasks = NodeTask->GetSubTasks();
	for (UEventflowSubTask* SubTask : Subtasks)
	{
		UQuestSubTask* QSub = Cast<UQuestSubTask>(SubTask);
		if (QSub)
		{
			TInstancedStruct<FQuestRuntimeData> QData;
			QSub->PullQuestData(QData);

			SubtaskData += QSub->TaskName.ToString();

			FQuestRuntimeData* QDataPtr = QData.GetMutablePtr();
			if (QDataPtr)
			{
				SubtaskData += " : " + QDataPtr->Value;
			}

			SubtaskData += "\n";
		}
	}

	NewItem->SubTaskTextBlock->SetText(FText::FromString(SubtaskData.TrimStartAndEnd()));
}

void UQuestFocusUI::RenderNodeTask(UQuestEngine* Engine, UEventflowNodeTask* NodeTask, UQuestItemUI* NewItem)
{
	if (NodeTask)
	{
		UQuestPrimaryTask* QTask = Cast<UQuestPrimaryTask>(NodeTask);
		if (QTask->bCanCancel)
		{
			NewItem->CancelButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			NewItem->CancelButton->SetVisibility(ESlateVisibility::Collapsed);
		}

		NewItem->NodeTaskTextBlock->SetText(FText::FromName(NodeTask->TaskName));

		RenderSubTask(NodeTask, NewItem);

		TMap<FPrimaryAssetId, TObjectPtr<UQuestEngine>> GroupEngines;
		QuestSubsystem->GetQuestsByGroup(Engine->GetAsset()->GetPrimaryAssetId(), GroupEngines);

		RenderGroupQuests(GroupEngines, NewItem);
	}
}

void UQuestFocusUI::RenderMainQuests(UQuestEngine* Engine)
{
	if (!Engine)
	{
		return;
	}

	const FEventflowEntryData* Data = Engine->GetEntryData().GetPtr<FEventflowEntryData>();
	if (!Data) return;
	if (Data->GroupId.IsValid())
	{
		return;
	}

	UQuestAsset* Asset = Engine->GetAsset<UQuestAsset>();

	UQuestItemUI* NewItem = CreateWidget<UQuestItemUI>(this, ItemClass);
	NewItem->TitleTextBlock->SetText(Asset->Title);

	UEventflowNodeTask* NodeTask = Engine->GetNodeTask();
	if (!NodeTask) return;

	RenderNodeTask(Engine, NodeTask, NewItem);

	QuestPanel->AddChild(NewItem);
}


void UQuestFocusUI::RenderGroupSubTask(UEventflowNodeTask* NodeTask, UQuestItemUI* GroupItem)
{
	FString SubtaskData;

	const TArray<TObjectPtr<UEventflowSubTask>>& Subtasks = NodeTask->GetSubTasks();
	for (UEventflowSubTask* SubTask : Subtasks)
	{
		UQuestSubTask* QSub = Cast<UQuestSubTask>(SubTask);
		if (QSub)
		{
			TInstancedStruct<FQuestRuntimeData> QData;
			QSub->PullQuestData(QData);

			SubtaskData += QSub->TaskName.ToString();

			FQuestRuntimeData* QDataPtr = QData.GetMutablePtr();
			if (QDataPtr)
			{
				SubtaskData += " : " + QDataPtr->Value;
			}

			SubtaskData += "\n";
		}
	}

	GroupItem->SubTaskTextBlock->SetText(FText::FromString(SubtaskData.TrimStartAndEnd()));
}

void UQuestFocusUI::RenderGroupNodeTask(UQuestEngine* Engine, UEventflowNodeTask* NodeTask, UQuestItemUI* GroupItem)
{
	if (NodeTask)
	{
		UQuestPrimaryTask* QTask = Cast<UQuestPrimaryTask>(NodeTask);
		if (QTask->bCanCancel)
		{
			GroupItem->CancelButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			GroupItem->CancelButton->SetVisibility(ESlateVisibility::Collapsed);
		}

		GroupItem->NodeTaskTextBlock->SetText(FText::FromName(NodeTask->TaskName));
		RenderGroupSubTask(NodeTask, GroupItem);
	}
}

void UQuestFocusUI::RenderGroupQuests(const TMap<FPrimaryAssetId, TObjectPtr<UQuestEngine>>& GroupEngines, UQuestItemUI* NewItem)
{
	for (const TPair<FPrimaryAssetId, TObjectPtr<UQuestEngine>>& Kv : GroupEngines)
	{
		UQuestEngine* Engine = Kv.Value.Get();
		if (!Engine) continue;

		UQuestAsset* Asset = Engine->GetAsset<UQuestAsset>();

		UQuestItemUI* GroupItem = CreateWidget<UQuestItemUI>(this, ItemClass);
		GroupItem->TitleTextBlock->SetText(Asset->Title);

		UEventflowNodeTask* NodeTask = Engine->GetNodeTask();
		if (!NodeTask) continue;

		RenderGroupNodeTask(Engine, NodeTask, GroupItem);

		NewItem->GroupPanel->AddChild(GroupItem);
	}
}


void UQuestFocusUI::HandleOnQuestUpdated()
{
	QuestPanel->ClearChildren();

	UQuestEngine* Quest = QuestSubsystem->GetFocusedQuest();
	RenderMainQuests(Quest);
}

void UQuestFocusUI::HandleOnQuestCancelInput(const FInputActionValue& Value)
{
	if (IsValid(QuestSubsystem))
	{
		QuestSubsystem->CancelFocusedTask();
	}
}


void UQuestFocusUI::NativeConstruct()
{
	Super::NativeConstruct();

	QuestSubsystem = UQuestSubsystem::Get(GetWorld());
	if (IsValid(QuestSubsystem))
	{
		QuestSubsystem->OnQuestFocusChanged.AddWeakLambda(this, [this]() { this->HandleOnQuestUpdated(); });
		QuestSubsystem->OnQuestDataChanged.AddWeakLambda(this, [this]() { this->HandleOnQuestUpdated(); });
		QuestSubsystem->GetOnEngineAdded().AddWeakLambda(this, [this](FPrimaryAssetId, UEventflowEngine*) { this->HandleOnQuestUpdated(); });
		QuestSubsystem->GetOnEngineRemoved().AddWeakLambda(this, [this](FPrimaryAssetId, UEventflowEngine*) { this->HandleOnQuestUpdated(); });
	}

	APlayerController* PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController))
	{
		UEnhancedInputComponent* ControllerInput = Cast<UEnhancedInputComponent>((PlayerController->InputComponent));
		if (IsValid(ControllerInput))
		{
			if (IsValid(QuestCancel))
			{
				FEnhancedInputActionEventBinding& Binding = ControllerInput->BindAction(QuestCancel, ETriggerEvent::Triggered, this, &UQuestFocusUI::HandleOnQuestCancelInput);
				InputHandles.Add(Binding.GetHandle());
			}
		}
	}
}

void UQuestFocusUI::NativeDestruct()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController))
	{
		UEnhancedInputComponent* ControllerInput = Cast<UEnhancedInputComponent>((PlayerController->InputComponent));
		if (IsValid(ControllerInput))
		{
			for (const uint8& Handle : InputHandles)
			{
				ControllerInput->RemoveBindingByHandle(Handle);
			}
			InputHandles.Empty();
		}
	}

	QuestSubsystem = UQuestSubsystem::Get(GetWorld());
	if (IsValid(QuestSubsystem))
	{
		QuestSubsystem->OnQuestFocusChanged.RemoveAll(this);
		QuestSubsystem->OnQuestDataChanged.RemoveAll(this);
		QuestSubsystem->GetOnEngineAdded().RemoveAll(this);
		QuestSubsystem->GetOnEngineRemoved().RemoveAll(this);
	}

	Super::NativeDestruct();
}

