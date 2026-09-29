// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/DialogueEdGraphSchema.h"

// Project Headers
#include "Graph/DialogueEdGraphNode.h"


TMap<FName, UClass*> UDialogueEdGraphSchema::GetRegisteredNodeClasses() const
{
	TMap<FName, UClass*> NodeClasses;

	NodeClasses.Add(UDialogueEdNode_Begin::StaticClass()->GetFName(), UDialogueEdNode_Begin::StaticClass());
	NodeClasses.Add(UDialogueEdNode_End::StaticClass()->GetFName(), UDialogueEdNode_End::StaticClass());
	NodeClasses.Add(UDialogueEdNode_Dialogue::StaticClass()->GetFName(), UDialogueEdNode_Dialogue::StaticClass());
	NodeClasses.Add(UDialogueEdNode_Branch::StaticClass()->GetFName(), UDialogueEdNode_Branch::StaticClass());

	return NodeClasses;
}

