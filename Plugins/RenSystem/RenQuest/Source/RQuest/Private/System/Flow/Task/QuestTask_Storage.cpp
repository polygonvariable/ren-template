// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Flow/Task/QuestTask_Storage.h"

// Project Headers
#include "Data/QuestAsset.h"
#include "System/Flow/QuestEngine.h"
#include "System/QuestStorageManager.h"
#include "System/QuestSubsystem.h"


void UQuestTask_CheckStorage::OnActive(EFSMState PreviousState)
{
	UQuestSubsystem* Subsystem = UQuestSubsystem::Get(GetWorld());
	check(Subsystem);

	UQuestStorageManager* StorageManager = Subsystem->GetStorageManager();
	check(StorageManager);

	UQuestEngine* Engine = GetOwningEngine<UQuestEngine>();
	check(Engine);

	const UQuestAsset* Asset = Engine->GetAsset<UQuestAsset>();
	check(Asset);

	bool bComplete = false;
	bool bSuccess = StorageManager->ContainsCompletedQuest(Asset->GetPrimaryAssetId(), bComplete);
	if (!bSuccess)
	{
		ensureMsgf(false, TEXT("Failed to get quest result"));
		return;
	}

	ModifyTransitionData(
		[bComplete](TInstancedStruct<FEventflowNodeTransitionData>& TransitionData)
		{
			FEventflowNodeTransitionData& Data = TransitionData.GetMutable();
			Data.NextNodeIndex = bComplete ? 0 : 1;

		}
	);

	Finish(EFSMResult::Success);
}


void UQuestTask_Checkpoint::OnActive(EFSMState PreviousState)
{
	UQuestSubsystem* Subsystem = UQuestSubsystem::Get(GetWorld());
	check(Subsystem);

	UQuestStorageManager* StorageManager = Subsystem->GetStorageManager();
	check(StorageManager);

	UQuestEngine* Engine = GetOwningEngine<UQuestEngine>();
	check(Engine);

	const UQuestAsset* Asset = Engine->GetAsset<UQuestAsset>();
	check(Asset);

	StorageManager->FindAddActiveQuest(Asset->GetPrimaryAssetId(), GetOwningNodeId());

	Finish(EFSMResult::Success);
}


void UQuestTask_Complete::OnActive(EFSMState PreviousState)
{
	UQuestSubsystem* Subsystem = UQuestSubsystem::Get(GetWorld());
	check(Subsystem);

	UQuestStorageManager* StorageManager = Subsystem->GetStorageManager();
	check(StorageManager);

	UQuestEngine* Engine = GetOwningEngine<UQuestEngine>();
	check(Engine);

	const UQuestAsset* Asset = Engine->GetAsset<UQuestAsset>();
	check(Asset);

	if (!StorageManager->CompleteActiveQuest(Asset->GetPrimaryAssetId()))
	{
		ensureMsgf(false, TEXT("Failed to complete quest"));
		return;
	}

	Finish(EFSMResult::Success);
}

