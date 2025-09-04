// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Quest/QuestBlueprint.h"

// Engine Headers

// Project Headers
#include "RenEventflow/Public/EventflowNode.h"

#include "RenQuest/Public/Quest/QuestInterface.h"



bool UQuestBlueprint::BP_RegisterObjective(UObject* Object, UEventflowNode* Node)
{
    return RegisterObjective(Object, Node) != nullptr;
}

IQuestObjectiveInterface* UQuestBlueprint::RegisterObjective(UObject* Object, UEventflowNode* Node)
{
    if (!Object || !Node)
    {
        return nullptr;
    }

    IQuestObjectiveInterface* ObjectiveInterface = Cast<IQuestObjectiveInterface>(Object);
    if (!ObjectiveInterface)
    {
        return nullptr;
    }

    ObjectiveTasks.Add(Object);

    ObjectiveInterface->SetObjectiveNode(Node);
    ObjectiveInterface->InitializeObjective();

    return ObjectiveInterface;
}

UEventflowNode* UQuestBlueprint::UnregisterObjective(UObject* Object)
{
    IQuestObjectiveInterface* ObjectiveInterface = Cast<IQuestObjectiveInterface>(Object);
    if (!ObjectiveInterface)
    {
		return nullptr;
    }

    ObjectiveTasks.Remove(Object);

    UEventflowNode* Node = ObjectiveInterface->GetObjectiveNode();

    ObjectiveInterface->GetOnQuestObjectiveCompleted().Unbind();
    ObjectiveInterface->DestroyObjective();

    return Node;
}

void UQuestBlueprint::UnregisterBlueprint()
{
    for (const auto& Task : ObjectiveTasks)
	{
		UnregisterObjective(Task.Get());
	}
    ObjectiveTasks.Empty();

    Super::UnregisterBlueprint();
}

