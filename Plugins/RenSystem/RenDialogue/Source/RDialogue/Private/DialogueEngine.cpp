// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialogueEngine.h"

// Project Headers
#include "DialogueNodeData.h"
#include "DialogueWidget.h"
#include "EventflowData.h"
#include "EventflowNode.h"
#include "EventflowNodeData.h"


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

