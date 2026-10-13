// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Flow/Task/QuestTask_ExternalTask.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

// Project Headers
#if WITH_EDITOR
#include "Core/QuestSettings.h"
#endif
#include "Core/QuestEvents.h"
#include "EventflowAsset.h"
#include "EventflowEngine.h"
#include "EventflowEngineProvider.h"
#include "EventflowLibrary.h"


void UQuestTask_ExternalTask::CopyFromAsset(const UEventflowTask* Template)
{
	Super::CopyFromAsset(Template);

	const UQuestTask_ExternalTask* Task = Cast<UQuestTask_ExternalTask>(Template);
	checkf(Task, TEXT("Invalid task template"));

	ExternalAsset = Task->ExternalAsset;
	ExternalPins = Task->ExternalPins;
}

#if WITH_EDITOR
void UQuestTask_ExternalTask::AppendAssetBundleData(FAssetBundleData& AssetBundle)
{
	Super::AppendAssetBundleData(AssetBundle);

	const UQuestSettings* Settings = UQuestSettings::Get();
	const FName& BundleName = Settings->BundleName;

	if (!ExternalAsset.IsNull())
	{
		AssetBundle.AddBundleAsset(BundleName, ExternalAsset.ToSoftObjectPath().GetAssetPath());
	}
}
EDataValidationResult UQuestTask_ExternalTask::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (ExternalAsset.IsNull())
	{
		Context.AddError(FText::FromString("External eventflow asset is invalid"));
		return EDataValidationResult::Invalid;
	}

	if (ExternalPins.IsEmpty())
	{
		Context.AddError(FText::FromString("External evetflow have invalid exit pins"));
		return EDataValidationResult::Invalid;
	}

	return Result;
}
#endif

void UQuestTask_ExternalTask::HandleOnEngineRemoved(FPrimaryAssetId AssetId, UEventflowEngine* Engine)
{
	const UEventflowAsset* TaskAsset = ExternalAsset.Get();
	checkf(IsValid(TaskAsset), TEXT("Failed to get external task asset"));

	FPrimaryAssetId TaskAssetId = TaskAsset->GetPrimaryAssetId();
	if (TaskAssetId != AssetId)
	{
		return;
	}

	IEventflowEngineProvider* EngineProvider = FEventflowLibrary::GetEngineProvider(GetWorld(), TaskAssetId);
	checkf(EngineProvider, TEXT("Failed to get external task engine provider"));

	EngineProvider->GetOnEngineRemoved().RemoveAll(this);

	const TInstancedStruct<FEventflowReturnData>& ReturnData = Engine->GetReturnData();
	if (!ReturnData.IsValid())
	{
		return;
	}

	const FEventflowReturnData& Data = ReturnData.Get();
	FGuid ExitId = Data.ExitNodeId;

	int Index = ExternalPins.IndexOfByPredicate([ExitId](const FGuid& NodeId) { return NodeId == ExitId; });
	if (Index >= 0)
	{
		ModifyTransitionData(
			[Index](TInstancedStruct<FEventflowNodeTransitionData>& TransitionData)
			{
				FEventflowNodeTransitionData& Data = TransitionData.GetMutable();
				Data.NextNodeIndex = Index;
			}
		);

		TWeakObjectPtr<UQuestTask_ExternalTask> WeakThis(this);

		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		TimerManager.SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this,
			[WeakThis]()
			{
				UQuestTask_ExternalTask* This = WeakThis.Get();
				checkf(This, TEXT("Failed to resolve task pointer"));

				This->Finish(EFSMResult::Success);
			}
		));
	}
}

FPrimaryAssetId UQuestTask_ExternalTask::GetGroupId() const
{
	UEventflowEngine* Engine = GetOwningEngine();
	checkf(IsValid(Engine), TEXT("Quest tasks contains invalid owning engine"));

	const FEventflowEntryData* Data = Engine->GetEntryData().GetPtr<FEventflowEntryData>();
	checkf(Data, TEXT("Quest engine contains invalid entry data"));

	if (Data->GroupId.IsValid())
	{
		return Data->GroupId;
	}

	UEventflowAsset* Asset = Engine->GetAsset();
	checkf(IsValid(Asset), TEXT("Quest engine contains invalid asset"));

	return Asset->GetPrimaryAssetId();
}

void UQuestTask_ExternalTask::OnInitialized(EFSMState PreviousState)
{
	Super::OnInitialized(PreviousState);
	Load();
}

void UQuestTask_ExternalTask::OnLoaded(EFSMState PreviousState)
{
	Ready();
}

void UQuestTask_ExternalTask::OnReady(EFSMState PreviousState)
{
	Active();
}

void UQuestTask_ExternalTask::OnActive(EFSMState PreviousState)
{
	FQuestEvents::DataChanged(GetWorld());

	const UEventflowAsset* TaskAsset = ExternalAsset.Get();
	checkf(IsValid(TaskAsset), TEXT("Failed to get external task asset"));

	FPrimaryAssetId TaskAssetId = TaskAsset->GetPrimaryAssetId();
	IEventflowEngineProvider* EngineProvider = FEventflowLibrary::GetEngineProvider(GetWorld(), TaskAssetId);
	checkf(EngineProvider, TEXT("Failed to get external task engine provider"));

	TInstancedStruct<FEventflowEntryData> EntryData;
	FEventflowEntryData& Data = EntryData.InitializeAs<FEventflowEntryData>();
	Data.GroupId = GetGroupId();

	EngineProvider->GetOnEngineRemoved().AddUObject(this, &UQuestTask_ExternalTask::HandleOnEngineRemoved);
	EngineProvider->StartEventflow(TaskAssetId, EntryData);
}

void UQuestTask_ExternalTask::OnReset()
{
	const UEventflowAsset* TaskAsset = ExternalAsset.Get();
	if (IsValid(TaskAsset))
	{
		FPrimaryAssetId TaskAssetId = TaskAsset->GetPrimaryAssetId();
		IEventflowEngineProvider* EngineProvider = FEventflowLibrary::GetEngineProvider(GetWorld(), TaskAssetId);
		if (EngineProvider)
		{
			EngineProvider->GetOnEngineRemoved().RemoveAll(this);
		}
	}

	Super::OnReset();
}

