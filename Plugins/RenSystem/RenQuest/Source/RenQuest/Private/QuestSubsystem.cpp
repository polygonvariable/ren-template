// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "QuestSubsystem.h"

// Engine Headers

// Project Headers
#include "RenEventflow/Public/EventflowData.h"
#include "RenEventflow/Public/EventflowNode.h"
#include "RenEventflow/Public/EventflowNodeData.h"
#include "RenEventflow/Public/EventflowNodeTask.h"

#include "RenQuest/Public/QuestAsset.h"
#include "RenQuest/Public/StoryboardAsset.h"




void UObjectPoolX::InitializePool(TSubclassOf<UObject> InClass, int32 InitialSize)
{
	PooledClass = InClass;
}

UObject* UObjectPoolX::CreateNewObject()
{
	if (!PooledClass) return nullptr;

	return NewObject<UObject>(this, PooledClass);
}

UObject* UObjectPoolX::Acquire()
{
	if (AvailableObjects.Num() == 0)
	{
		return CreateNewObject();
	}

	return AvailableObjects.Pop();
}

void UObjectPoolX::Release(UObject* Object)
{
	if (Object)
	{
		AvailableObjects.Add(Object);
	}
}






void UQuestSubsystem::StartQuest(UQuestGraph* QuestGraph)
{
	if (!QuestGraph)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestGraph is invalid"));
		return;
	}

	if (IsQuestActive(QuestGraph) || IsQuestCompleted(QuestGraph))
	{
		UE_LOG(LogTemp, Warning, TEXT("Quest is already active or completed"));
		return;
	}

	UEventflowData* QuestData = QuestGraph->GraphData;
	if (!QuestData)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestData is invalid"));
		return;
	}

	UEventflowNode* EntryNode = QuestData->GetEntryNode();
	if (!EntryNode || !EntryNode->NodeData || !EntryNode->NodeData->NodeTask)
	{
		UE_LOG(LogTemp, Warning, TEXT("EntryNode, NodeData or NodeTask is invalid"));
		return;
	}
	
	TSubclassOf<UEventflowNodeTask> TaskClass = EntryNode->NodeData->NodeTask;
	if (TaskClass)
	{
		UEventflowNodeTask* NodeTask = NewObject<UEventflowNodeTask>(this, TaskClass);
		QuestTasks.Add(QuestGraph, NodeTask);

		NodeTask->SetOwnedNode(EntryNode);
		NodeTask->SetOwnedGraphAsset(QuestGraph);
		NodeTask->OnActionStopped.BindWeakLambda(this, [this, NodeTask](bool bSuccess)
			{
				if (bSuccess)
				{
					// proceed to next node
				}

				/*if (this->NodeTaskPool && NodeTask)
				{
					this->NodeTaskPool->Release(NodeTask);
				}*/
			}
		);
		NodeTask->StartAction();

		return;
	}
}

void UQuestSubsystem::EndQuest(UQuestGraph* QuestGraph)
{
	if (!IsQuestActive(QuestGraph))
	{
		UE_LOG(LogTemp, Warning, TEXT("Quest is not active"));
		return;
	}

	ActiveQuests.Remove(QuestGraph);
	CompletedQuests.Add(QuestGraph);

	OnQuestCompleted.Broadcast(QuestGraph);
}



bool UQuestSubsystem::IsQuestActive(UQuestGraph* QuestGraph) const
{
	return ActiveQuests.Contains(QuestGraph);
}

bool UQuestSubsystem::IsQuestCompleted(UQuestGraph* QuestGraph) const
{
	return CompletedQuests.Contains(QuestGraph);
}



bool UQuestSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UQuestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//NodeTaskPool = NewObject<UObjectPoolX>(this);
	//NodeTaskPool->InitializePool(UQuestNodeTask::StaticClass(), 10);
}

void UQuestSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	Super::OnWorldComponentsUpdated(InWorld);
}

void UQuestSubsystem::Deinitialize()
{
	/*if (NodeTaskPool)
	{
		NodeTaskPool->MarkAsGarbage();
	}*/
	Super::Deinitialize();
}
