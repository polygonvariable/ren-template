// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/DialogueEdGraphSchema.h"

// Project Headers
#include "Graph/DialogueEdGraphNode.h"


TMap<FName, UClass*> UDialogueEdGraphSchema::GetRegisteredNodeClasses() const
{
	TMap<FName, UClass*> NodeClasses;

	NodeClasses.Add(UDialogueEdBeginNode::StaticClass()->GetFName(), UDialogueEdBeginNode::StaticClass());
	NodeClasses.Add(UDialogueEdEndNode::StaticClass()->GetFName(), UDialogueEdEndNode::StaticClass());
	NodeClasses.Add(UDialogueEdDialogNode::StaticClass()->GetFName(), UDialogueEdDialogNode::StaticClass());
	NodeClasses.Add(UDialogueEdBranchNode::StaticClass()->GetFName(), UDialogueEdBranchNode::StaticClass());

	return NodeClasses;
}

