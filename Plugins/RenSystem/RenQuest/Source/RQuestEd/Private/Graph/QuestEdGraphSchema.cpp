// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/QuestEdGraphSchema.h"

// Project Headers
#include "Graph/QuestEdGraphNode.h"


TMap<FName, UClass*> UQuestEdGraphSchema::GetRegisteredNodeClasses() const
{
	TMap<FName, UClass*> NodeClasses;

	TArray<UClass*> DerivedClasses;
	GetDerivedClasses(UQuestEdGraphNode::StaticClass(), DerivedClasses, true);

	for (UClass* Class : DerivedClasses)
	{
		if (!IsValid(Class))
		{
			continue;
		}

		if (Class->HasAnyClassFlags(CLASS_Abstract | CLASS_Deprecated | CLASS_NewerVersionExists))
		{
			continue;
		}

		if (!Class->HasMetaData(TEXT("QuestNode")))
		{
			continue;
		}

		NodeClasses.Add(Class->GetFName(), Class);
	}

	return NodeClasses;
}

