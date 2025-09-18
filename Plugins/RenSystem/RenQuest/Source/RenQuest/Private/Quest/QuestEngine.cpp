// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Quest/QuestEngine.h"

// Engine Headers

// Project Headers
#include "RenEventflow/Public/EventflowData.h"
#include "RenEventflow/Public/EventflowNode.h"
#include "RenEventflow/Public/EventflowNodeData.h"

#include "RenQuest/Public/Quest/QuestAsset.h"
#include "RenQuest/Public/Quest/QuestSubsystem.h"




void UQuestEngine::SetEntryIds(const TArray<FGuid>& NodeIds)
{
	EntryIds = NodeIds;
}

UQuestAsset* UQuestEngine::GetQuestAsset() const
{
	return Cast<UQuestAsset>(CurrentAsset);
}

UQuestSubsystem* UQuestEngine::GetQuestSubsystem() const
{
	return Cast<UQuestSubsystem>(GetOuter());
}

void UQuestEngine::HandleOnNodeReached(UEventflowNode* Node)
{
	UQuestAsset* QuestAsset = GetQuestAsset();
	if (!IsValid(QuestAsset))
	{
		return;
	}

	if (!QuestAsset->bIsResumable)
	{
		ExecuteNode(Node);
		return;
	}

	UQuestSubsystem* QuestSubsystem = GetQuestSubsystem();
	if (!IsValid(QuestSubsystem) || !Node->NodeId.IsValid())
	{
		return;
	}

	if (QuestSubsystem->SetObjectiveToActive(CurrentAssetId, Node->NodeId))
	{
		ExecuteNode(Node);
	}
}

void UQuestEngine::HandleOnNodeExited(UEventflowNode* Node, bool bSuccess, int NextNodeIndex)
{
	UEventflowNodeData* NodeData = Node->NodeData;
	if (NodeData->NodeTags.Contains(TEXT("End")))
	{
		HandleOnGraphEnded();
		return;
	}

	UQuestAsset* QuestAsset = GetQuestAsset();
	if (!bSuccess || !IsValid(QuestAsset))
	{
		HandleOnGraphEnded();
		return;
	}

	if (!QuestAsset->bIsResumable)
	{
		ReachNextNode(Node, NextNodeIndex);
		return;
	}

	UQuestSubsystem* QuestSubsystem = GetQuestSubsystem();
	if (!IsValid(QuestSubsystem) || !Node->NodeId.IsValid())
	{
		return;
	}

	if (QuestSubsystem->SetObjectiveToCompleted(CurrentAssetId, Node->NodeId))
	{
		ReachNextNode(Node, NextNodeIndex);
	}
}

void UQuestEngine::HandleOnGraphStarted()
{
	if (EntryIds.Num() > 0)
	{
		for (const FGuid& EntryId : EntryIds)
		{
			ReachNodeById(EntryId);
		}
	}
	else
	{
		ReachEntryNode();
	}
}

void UQuestEngine::HandleOnGraphEnded()
{
	UQuestSubsystem* QuestSubsystem = GetQuestSubsystem();
	if (IsValid(QuestSubsystem))
	{
		QuestSubsystem->EndQuest(CurrentAssetId);
	}
}

