// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Flow/Task/QuestTask_GameplayContext.h"

// Project Headers
#include "Core/AssetInstanceLibrary.h"
#include "Core/Interface/AssetInstanceCollection.h"
#include "Core/Type/AssetDetail.h"
#include "Core/Type/AssetFilterProperty.h"


void UQuestTask_GameplayContext::OnActive(EFSMState PreviousState)
{
	UWorld* World = GetWorld();

	for (TInstancedStruct<FGameplayContextAction>& Context : Contexts)
	{
		FGameplayContextAction* Data = Context.GetMutablePtr();
		if (Data)
		{
			Data->Execute(World, nullptr, nullptr);
		}
	}

	Finish(EFSMResult::Success);
}


void UQuestTask_AddAssetInstance::OnActive(EFSMState PreviousState)
{
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	IAssetInstanceCollection* InstanceCollection = FAssetInstanceLibrary::GetInstanceCollection(GameInstance, AssetType, SourceId);
	if (!InstanceCollection)
	{
		ensureAlwaysMsgf(false, TEXT("Failed to get asset instance collection for inventory"));
		return;
	}

	InstanceCollection->AddInstances(Items, 1);

	Finish(EFSMResult::Success);
}


void UQuestTask_ContainAssetInstance::OnActive(EFSMState PreviousState)
{
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	IAssetInstanceCollection* InstanceCollection = FAssetInstanceLibrary::GetInstanceCollection(GameInstance, AssetType, SourceId);
	if (!InstanceCollection)
	{
		ensureAlwaysMsgf(false, TEXT("Failed to get asset instance collection for inventory"));
		return;
	}

	bool bResult = false;
	if (bContainsAll)
	{
		bResult = InstanceCollection->ContainInstances(Items, 1);
	}
	else
	{
		FPrimaryAssetId AssetId;
		int Quantity = 0;

		bResult = InstanceCollection->ContainAnyInstances(Items, 1, AssetId, Quantity);
	}

	ModifyTransitionData(
		[bResult](TInstancedStruct<FEventflowNodeTransitionData>& TransitionData)
		{
			FEventflowNodeTransitionData& Data = TransitionData.GetMutable();
			Data.NextNodeIndex = bResult ? 0 : 1;
		}
	);

	Finish(EFSMResult::Success);
}

