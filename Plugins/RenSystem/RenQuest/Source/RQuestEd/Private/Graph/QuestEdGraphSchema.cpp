// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/QuestEdGraphSchema.h"

// Project Headers
#include "Graph/QuestEdGraphNode.h"


TMap<FName, UClass*> UQuestEdGraphSchema::GetRegisteredNodeClasses() const
{
	TMap<FName, UClass*> NodeClasses;

	NodeClasses.Add(UQuestEdNode_Begin::StaticClass()->GetFName(), UQuestEdNode_Begin::StaticClass());
	NodeClasses.Add(UQuestEdNode_End::StaticClass()->GetFName(), UQuestEdNode_End::StaticClass());
	NodeClasses.Add(UQuestEdNode_SpawnMarker::StaticClass()->GetFName(), UQuestEdNode_SpawnMarker::StaticClass());
	NodeClasses.Add(UQuestEdNode_ConditionalSpawnMarker::StaticClass()->GetFName(), UQuestEdNode_ConditionalSpawnMarker::StaticClass());
	NodeClasses.Add(UQuestEdNode_Reroute::StaticClass()->GetFName(), UQuestEdNode_Reroute::StaticClass());
	NodeClasses.Add(UQuestEdNode_ExternalTask::StaticClass()->GetFName(), UQuestEdNode_ExternalTask::StaticClass());

	return NodeClasses;
}

