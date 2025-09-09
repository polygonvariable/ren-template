// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Quest/QuestEngine.h"

// Engine Headers

// Project Headers
#include "RenEventflow/Public/EventflowNode.h"
#include "RenEventflow/Public/EventflowData.h"

#include "RenQuest/Public/Quest/QuestAsset.h"
#include "RenQuest/Public/Quest/QuestSubsystem.h"




void UQuestEngine::SetEntryId(FGuid NodeId)
{
	EntryId = NodeId;
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
	UQuestSubsystem* QuestSubsystem = GetQuestSubsystem();
	if (!IsValid(QuestSubsystem) || !Node->NodeId.IsValid())
	{
		return;
	}

	if (QuestSubsystem->SetObjectiveToActive(GetQuestAsset(), Node->NodeId))
	{
		ExecuteNode(Node);
	}
}

void UQuestEngine::HandleOnNodeExited(UEventflowNode* Node, bool bSuccess)
{
	if (!bSuccess)
	{
		return;
	}

	UQuestSubsystem* QuestSubsystem = GetQuestSubsystem();
	if (!IsValid(QuestSubsystem) || !Node->NodeId.IsValid())
	{
		return;
	}

	if (QuestSubsystem->SetObjectiveToCompleted(GetQuestAsset(), Node->NodeId))
	{
		ReachImmediateNextNode();
	}
}

void UQuestEngine::HandleOnGraphEnded()
{
	UQuestSubsystem* QuestSubsystem = GetQuestSubsystem();
	if (IsValid(QuestSubsystem))
	{
		QuestSubsystem->EndQuest(GetQuestAsset());
	}
}

void UQuestEngine::HandleOnEngineInitialized()
{
	if (EntryId.IsValid())
	{
		ReachNode(EntryId);
	}
	else
	{
		ReachEntryNode();
	}
}
