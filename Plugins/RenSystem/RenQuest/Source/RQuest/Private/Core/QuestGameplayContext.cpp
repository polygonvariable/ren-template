// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/QuestGameplayContext.h"

// Project Headers
#include "Actor/QuestObjectiveActor.h"
#include "Core/EventflowIO.h"
#include "System/QuestSubsystem.h"


bool FGameplayContext_StartQuest::Execute(UWorld* World, UObject* Owner, UObject* Instigator)
{
	if (!::IsValid(World) || !AssetId.IsValid())
	{
		return false;
	}

	UQuestSubsystem* Subsystem = UQuestSubsystem::Get(World);
	if (!::IsValid(Subsystem))
	{
		return false;
	}

	TInstancedStruct<FEventflowEntryData> EntryData;
	FEventflowEntryData& Data = EntryData.InitializeAs<FEventflowEntryData>();

	Subsystem->StartQuest(AssetId, EntryData);
	return false;
};


bool FGameplayContext_QuestInteractionHandle::Execute(UWorld* World, UObject* Owner, UObject* Instigator)
{
	UActorComponent* Component = Cast<UActorComponent>(Owner);
	if (!::IsValid(Component))
	{
		return false;
	}

	AQuestInteractionMarker* InteractActor = Cast<AQuestInteractionMarker>(Component->GetOwner());
	if (!::IsValid(Component))
	{
		return false;
	}

	InteractActor->OnInteractionCompleted.ExecuteIfBound(EFSMResult::Success);
	return true;
}

