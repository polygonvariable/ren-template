// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Flow/Task/QuestGlobalTask_SpawnActor.h"

// Project Headers
#if WITH_EDITOR
#include "Core/QuestSettings.h"
#endif
#include "System/Flow/QuestEngine.h"


void UQuestGlobalTask_SpawnActor::UpdateActorTransform(FTransform Transform)
{
	ActorTransform = Transform;
	if (IsValid(Actor))
	{
		Actor->SetActorTransform(ActorTransform);
	}
}

void UQuestGlobalTask_SpawnActor::CopyFromAsset(const UEventflowTask* Template)
{
	UQuestGlobalTask::CopyFromAsset(Template);

	const UQuestGlobalTask_SpawnActor* Task = Cast<UQuestGlobalTask_SpawnActor>(Template);
	checkf(Task, TEXT("Global task contains invalid task template"));

	ActorClass = Task->ActorClass;
	ActorTransform = Task->ActorTransform;
}

#if WITH_EDITOR
void UQuestGlobalTask_SpawnActor::AppendAssetBundleData(FAssetBundleData& AssetBundle)
{
	UQuestGlobalTask::AppendAssetBundleData(AssetBundle);

	const UQuestSettings* Settings = UQuestSettings::Get();
	const FName& BundleName = Settings->BundleName;

	if (!ActorClass.IsNull())
	{
		AssetBundle.AddBundleAsset(BundleName, ActorClass.ToSoftObjectPath().GetAssetPath());
	}
}
#endif

void UQuestGlobalTask_SpawnActor::OnInitialized(EFSMState PreviousState)
{
	UQuestGlobalTask::OnInitialized(PreviousState);
	Load();
}

void UQuestGlobalTask_SpawnActor::OnLoaded(EFSMState PreviousState)
{
	Actor = GetWorld()->SpawnActorDeferred<AActor>(ActorClass.Get(), ActorTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (!ensureMsgf(IsValid(Actor), TEXT("Failed to spawn global task actor")))
	{
		return;
	}

	Actor->FinishSpawning(ActorTransform);
	Ready();
}

void UQuestGlobalTask_SpawnActor::OnReady(EFSMState PreviousState)
{
	if (!ensureMsgf(IsValid(Actor), TEXT("Global task actor is invalid")))
	{
		return;
	}

	Actor->SetActorHiddenInGame(false);
	Actor->SetActorEnableCollision(true);
	Active();
}

void UQuestGlobalTask_SpawnActor::OnFinished(EFSMResult Result)
{
	if (IsValid(Actor))
	{
		Actor->SetActorHiddenInGame(true);
		Actor->SetActorEnableCollision(false);
	}
}

void UQuestGlobalTask_SpawnActor::OnReset()
{
	if (IsValid(Actor))
	{
		Actor->Destroy();
	}
	Actor = nullptr;

	Super::OnReset();
}


void FOGTFragment_UpdateActorTransform::Execute(UQuestEngine* Engine)
{
	UQuestGlobalTask_SpawnActor* GlobalTask = Engine->EnsureGlobalTask<UQuestGlobalTask_SpawnActor>(TaskName);
	if (!ensureMsgf(IsValid(GlobalTask), TEXT("Quest global task is invalid or not of valid type")))
	{
		return;
	}
	GlobalTask->UpdateActorTransform(Transform);
}

