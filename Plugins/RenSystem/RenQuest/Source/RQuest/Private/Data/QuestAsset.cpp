// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/QuestAsset.h"

// Project Headers
#include "System/Flow/Task/QuestPrimaryTask.h"


#if WITH_EDITORONLY_DATA
void UQuestAsset::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();

	for (UQuestGlobalTask* Task : GlobalTasks)
	{
		if (IsValid(Task))
		{
			Task->AppendAssetBundleData(AssetBundleData);
		}
	}
}
#endif

UEventflowGlobalTask* UQuestAsset::GetGlobalTask(FName TaskName) const
{
	const TObjectPtr<UQuestGlobalTask>* FoundTask = GlobalTasks.FindByPredicate([TaskName](UQuestGlobalTask* Task) { return IsValid(Task) && Task->TaskName == TaskName; });
	if (!FoundTask)
	{
		return nullptr;
	}
	return FoundTask->Get();
}

FPrimaryAssetId UQuestAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("Quest"), GetFName());
}

