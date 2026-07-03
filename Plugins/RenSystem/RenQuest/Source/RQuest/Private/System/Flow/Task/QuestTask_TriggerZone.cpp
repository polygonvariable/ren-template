// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Flow/Task/QuestTask_TriggerZone.h"

// Engine Headers
#include "UObject/UnrealType.h"

// Project Headers
#include "Actor/QuestObjectiveActor.h"
#include "Core/QuestSettings.h"
#include "Task/EventflowSubTask.h"
#include "Util/ObjectUtil.h"


#if WITH_EDITOR

void UQuestTask_TriggerZone::PreDuplicate(FObjectDuplicationParameters& DupParams)
{
	//UClass* Class = TriggerZone.ActorClass.LoadSynchronous();
	//FInstancedPropertyBag& PropertyBag = TriggerZone.Properties;

	//FObjectUtil::CleanupBagProperties(Class, PropertyBag);

	Super::PreDuplicate(DupParams);
}

void UQuestTask_TriggerZone::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	//TArray<FString> ChangeList = { "ActorClass" };
	//FString ChangedName = PropertyChangedEvent.Property->GetName();

	//if (ChangeList.Contains(ChangedName))
	//{
	//	FInstancedPropertyBag& PropertyBag = TriggerZone.Properties;
	//	UClass* Class = TriggerZone.ActorClass.LoadSynchronous();

	//	FObjectUtil::CopyPropertiesToBag(Class, PropertyBag);
	//}
}

void UQuestTask_TriggerZone::AppendAssetBundleData(FAssetBundleData& AssetBundle)
{
	Super::AppendAssetBundleData(AssetBundle);

	const UQuestSettings* Settings = UQuestSettings::Get();
	const FName& BundleName = Settings->BundleName;

	const TSoftClassPtr<AQuestObjectiveActor>& ActorClass = TriggerZone.ActorClass;

	AssetBundle.AddBundleAsset(BundleName, ActorClass.ToSoftObjectPath().GetAssetPath());
}

#endif

void UQuestTask_TriggerZone::CopyFromAsset(const UEventflowTask* Template)
{
	Super::CopyFromAsset(Template);

	const UQuestTask_TriggerZone* Task = Cast<UQuestTask_TriggerZone>(Template);
	if (IsValid(Task))
	{
		TriggerZone = Task->TriggerZone;
	}
}

void UQuestTask_TriggerZone::OnInitialized(EFSMState PreviousStatus)
{
	Super::OnInitialized(PreviousStatus);
	Load();
}

void UQuestTask_TriggerZone::OnLoaded(EFSMState PreviousStatus)
{
	UClass* ActorClass = TriggerZone.ActorClass.Get();
	UWorld* World = GetWorld();

	RuntimeActor = World->SpawnActorDeferred<AQuestObjectiveActor>(ActorClass, TriggerZone.Transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (IsValid(RuntimeActor))
	{
		FObjectUtil::CopyPropertiesToInstance(TriggerZone.Properties, RuntimeActor);

		RuntimeActor->SetOwningTask(this);
		RuntimeActor->FinishSpawning(TriggerZone.Transform);
	}

	Ready();
}

void UQuestTask_TriggerZone::OnReady(EFSMState PreviousState)
{
	if (IsValid(RuntimeActor))
	{
		RuntimeActor->SetActorHiddenInGame(false);
		RuntimeActor->SetActorEnableCollision(true);
	}
}

void UQuestTask_TriggerZone::OnActive(EFSMState PreviousState)
{
	if (IsValid(RuntimeActor))
	{
		RuntimeActor->SetActorHiddenInGame(true);
		RuntimeActor->SetActorEnableCollision(false);
	}

	const TArray<TObjectPtr<UEventflowSubTask>>& Tasks = GetSubTasks();
	for (UEventflowSubTask* Task : Tasks)
	{
		if (IsValid(Task))
		{
			Task->Execute();
		}
	}
}

void UQuestTask_TriggerZone::OnEndActive(EFSMState NextState, EFSMResult Result)
{

}

void UQuestTask_TriggerZone::OnFinished(EFSMResult Result)
{

}

void UQuestTask_TriggerZone::OnRestart(EFSMState PreviousState, EFSMResult PreviousResult)
{
	const TArray<TObjectPtr<UEventflowSubTask>>& Tasks = GetSubTasks();
	for (UEventflowSubTask* Task : Tasks)
	{
		if (IsValid(Task))
		{
			Task->Restart();
		}
	}
}

void UQuestTask_TriggerZone::OnReset()
{
	if (IsValid(RuntimeActor))
	{
		RuntimeActor->SetOwningTask(nullptr);
		RuntimeActor->Destroy();
	}
	RuntimeActor = nullptr;

	Super::OnReset();
}

