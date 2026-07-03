// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/QuestEdGraphSchema.h"

// Project Headers
#include "Graph/QuestEdGraphNode.h"


TMap<FName, UClass*> UQuestEdGraphSchema::GetRegisteredNodeClasses() const
{
	TMap<FName, UClass*> NodeClasses;

	NodeClasses.Add(UQuestEdNode_GoToPlace::StaticClass()->GetFName(), UQuestEdNode_GoToPlace::StaticClass());
	NodeClasses.Add(UQuestEdNode_Begin::StaticClass()->GetFName(), UQuestEdNode_Begin::StaticClass());
	NodeClasses.Add(UQuestEdNode_End::StaticClass()->GetFName(), UQuestEdNode_End::StaticClass());
	NodeClasses.Add(UQuestEdNode_SubGraph::StaticClass()->GetFName(), UQuestEdNode_SubGraph::StaticClass());

	return NodeClasses;
}

