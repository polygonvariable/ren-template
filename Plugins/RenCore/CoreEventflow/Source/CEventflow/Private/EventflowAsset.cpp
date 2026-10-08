// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowAsset.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#include "UObject/ObjectSaveContext.h"
#endif

// Project Headers
#if WITH_EDITOR
#include "Task/EventflowNodeTask.h"
#endif


UEventflowGlobalTask* UEventflowAsset::GetGlobalTask(FName TaskName) const
{
	return nullptr;
}

#if WITH_EDITOR
void UEventflowAsset::PreSaveRoot(FObjectPreSaveRootContext ObjectSaveContext)
{
	Super::PreSaveRoot(ObjectSaveContext);

	GEngine->ForceGarbageCollection(true);
	UE_LOG(LogTemp, Warning, TEXT("Eventflow asset presave (force GC)"));
}

EDataValidationResult UEventflowAsset::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	for (const TPair<FGuid, FEventflowNode>& Kv : NodeCollection)
	{
		UEventflowNodeTask* Task = Kv.Value.Task;
		if (!IsValid(Task))
		{
			Context.AddError(FText::FromString("Invalid task instance in graph"));
			return EDataValidationResult::Invalid;
		}

		EDataValidationResult TaskResult = Task->IsDataValid(Context);
		if (TaskResult == EDataValidationResult::Invalid)
		{
			return EDataValidationResult::Invalid;
		}
	}

	return EDataValidationResult::Valid;
}

void UEventflowAsset::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();

	for (const TPair<FGuid, FEventflowNode>& Kv : NodeCollection)
	{
		UEventflowNodeTask* Task = Kv.Value.Task;
		if (IsValid(Task))
		{
			Task->AppendAssetBundleData(AssetBundleData);
		}
	}
}
#endif

