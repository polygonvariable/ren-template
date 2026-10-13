// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/QuestGameplayContext.h"

// Project Headers
#include "Actor/QuestObjectiveActor.h"
#include "Core/EventflowIO.h"
#include "System/QuestStorageManager.h"
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
	if (!::IsValid(InteractActor))
	{
		return false;
	}

	InteractActor->OnInteractionCompleted.ExecuteIfBound(EFSMResult::Success);
	return true;
}


bool FGameplayContext_QuestActorVisibility::Execute(UWorld* World, UObject* Owner, UObject* Instigator)
{
	UQuestSubsystem* Subsystem = UQuestSubsystem::Get(World);
	if (!::IsValid(Subsystem))
	{
		return false;
	}

	UQuestStorageManager* StorageManager = Subsystem->GetStorageManager();
	if (!::IsValid(StorageManager))
	{
		return false;
	}

	bool bResult = false;
	bool bSuccess = false;

	switch (Condition)
	{
	case EQuestActorVisibilityCondition::OnActive:
		bSuccess = StorageManager->ContainsActiveQuest(QuestId, bResult);
		break;
	case EQuestActorVisibilityCondition::OnCompleted:
		bSuccess = StorageManager->ContainsCompletedQuest(QuestId, bResult);
		break;
	}

	UActorComponent* Component = Cast<UActorComponent>(Owner);
	if (!::IsValid(Component))
	{
		return false;
	}

	AActor* Actor = Component->GetOwner();
	if (!::IsValid(Actor))
	{
		return false;
	}

	if (!bSuccess)
	{
		Actor->SetActorHiddenInGame(true);
		Actor->SetActorEnableCollision(false);
		return false;
	}

	if (bResult)
	{
		Actor->SetActorHiddenInGame(bHidden);
		Actor->SetActorEnableCollision(!bHidden);
	}
	else
	{
		Actor->SetActorHiddenInGame(!bHidden);
		Actor->SetActorEnableCollision(bHidden);
	}

	return true;
}

