// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialogueEngine.h"

// Engine Headers

// Project Headers
#include "RenEventflow/Public/EventflowData.h"
#include "RenEventflow/Public/EventflowNode.h"
#include "RenEventflow/Public/EventflowNodeData.h"

#include "RenDialogue/Public/DialogueWidget.h"
#include "RenDialogue/Public/DialogueNodeData.h"




void UDialogueEngine::HandleOnNodeReached(UEventflowNode* Node)
{
	if (!IsValid(Node))
	{
		HandleOnGraphEnded();
		return;
	}

	if (IsValid(DialogueWidget))
	{
		DialogueWidget->SetDialogueContent(Node);
	}
}

void UDialogueEngine::HandleOnNodeExited(UEventflowNode* Node, bool bSuccess, int NextNodeIndex)
{

}

void UDialogueEngine::HandleOnGraphStarted()
{
	DialogueWidget = Cast<UDialogueWidget>(GetOuter());
}

void UDialogueEngine::HandleOnGraphEnded()
{
	DialogueWidget = nullptr;
}

