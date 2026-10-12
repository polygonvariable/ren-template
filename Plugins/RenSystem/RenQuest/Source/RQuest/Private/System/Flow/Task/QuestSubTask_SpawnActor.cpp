// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Flow/Task/QuestSubTask_SpawnActor.h"

// Project Headers
#if WITH_EDITOR
#include "Core/QuestSettings.h"
#endif
#include "Core/QuestEvents.h"
#include "Actor/QuestObjectiveActor.h"
#include "MiscLibrary.h"


UQuestTask_SpawnLocation::UQuestTask_SpawnLocation()
{
}


void UQuestTask_SpawnLocation::CopyFromAsset(const UEventflowTask* Template)
{
	Super::CopyFromAsset(Template);

	const UQuestTask_SpawnLocation* Task = Cast<UQuestTask_SpawnLocation>(Template);
	checkf(Task, TEXT("Sub task have invalid task template"));

	MarkerActor = Task->MarkerActor;
	MarkerClass = Task->MarkerClass;
	MarkerTransform = Task->MarkerTransform;
}

#if WITH_EDITOR
void UQuestTask_SpawnLocation::AppendAssetBundleData(FAssetBundleData& AssetBundle)
{
	Super::AppendAssetBundleData(AssetBundle);

	const UQuestSettings* Settings = UQuestSettings::Get();
	const FName& BundleName = Settings->BundleName;

	if (!MarkerClass.IsNull())
	{
		AssetBundle.AddBundleAsset(BundleName, MarkerClass.ToSoftObjectPath().GetAssetPath());
	}
}
#endif

void UQuestTask_SpawnLocation::HandleOnInteractionCompleted(EFSMResult Result)
{
	Active();
}


void UQuestTask_SpawnLocation::OnInitialized(EFSMState PreviousState)
{
	Super::OnInitialized(PreviousState);
	FMiscLibrary::NextFrame(this, &UQuestTask_SpawnLocation::Load);
}

void UQuestTask_SpawnLocation::OnLoaded(EFSMState PreviousState)
{
	MarkerActor = GetWorld()->SpawnActorDeferred<AQuestObjectiveMarker>(MarkerClass.Get(), MarkerTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (IsValid(MarkerActor))
	{
		MarkerActor->OnInteractionCompleted.BindUObject(this, &UQuestTask_SpawnLocation::HandleOnInteractionCompleted);
		MarkerActor->FinishSpawning(MarkerTransform);
		Ready();
	}
}

void UQuestTask_SpawnLocation::OnReady(EFSMState PreviousState)
{
	if (IsValid(MarkerActor))
	{
		MarkerActor->SetActorHiddenInGame(false);
		MarkerActor->SetActorEnableCollision(true);
	}

	FQuestEvents::DataChanged(GetWorld());
}

void UQuestTask_SpawnLocation::OnActive(EFSMState PreviousState)
{
	Finish(EFSMResult::Success);
}

void UQuestTask_SpawnLocation::OnFinished(EFSMResult Result)
{
	if (IsValid(MarkerActor))
	{
		MarkerActor->SetActorHiddenInGame(true);
		MarkerActor->SetActorEnableCollision(false);
	}
}

void UQuestTask_SpawnLocation::OnReset()
{
	if (IsValid(MarkerActor))
	{
		MarkerActor->Destroy();
	}
	MarkerActor = nullptr;

	Super::OnReset();
}
