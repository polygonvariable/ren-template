// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Flow/Task/QuestTask_GoToLocation.h"

// Engine Headers
#include "UObject/UnrealType.h"

// Project Headers
#include "Actor/QuestObjectiveActor.h"
#include "Core/QuestSettings.h"
#include "Util/ObjectUtil.h"


void UQuestTask_GoToLocation::CopyFromAsset(const UEventflowTask* Template)
{
	Super::CopyFromAsset(Template);

	const UQuestTask_GoToLocation* Task = Cast<UQuestTask_GoToLocation>(Template);
	if (IsValid(Task))
	{
		ActorData = Task->ActorData;
	}
}

#if WITH_EDITOR

void UQuestTask_GoToLocation::AppendAssetBundleData(FAssetBundleData& AssetBundle)
{
	const UQuestSettings* Settings = UQuestSettings::Get();
	const FName& BundleName = Settings->BundleName;

	const TSoftClassPtr<AQuestObjectiveActor>& ActorClass = ActorData.ActorClass;

	AssetBundle.AddBundleAsset(BundleName, ActorClass.ToSoftObjectPath().GetAssetPath());
}

void UQuestTask_GoToLocation::PreDuplicate(FObjectDuplicationParameters& DupParams)
{
	Super::PreDuplicate(DupParams);
}

void UQuestTask_GoToLocation::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif

void UQuestTask_GoToLocation::HandleOnDestinationReached()
{
	Finish(EFSMResult::Success);
}

void UQuestTask_GoToLocation::OnInitialized(EFSMState PreviousState)
{
	Super::OnInitialized(PreviousState);

	Load();
}

void UQuestTask_GoToLocation::OnLoaded(EFSMState PreviousState)
{
	UClass* ActorClass = ActorData.ActorClass.Get();
	UWorld* World = GetWorld();

	RuntimeActor = World->SpawnActorDeferred<AQuestObjectiveActor>(ActorClass, ActorData.Transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (IsValid(RuntimeActor))
	{
		FObjectUtil::CopyPropertiesToInstance(ActorData.Properties, RuntimeActor);

		RuntimeActor->OnCompleted.BindUObject(this, &UQuestTask_GoToLocation::HandleOnDestinationReached);
		RuntimeActor->FinishSpawning(ActorData.Transform);
	}

	Ready();
}

void UQuestTask_GoToLocation::OnReady(EFSMState PreviousState)
{
	if (IsValid(RuntimeActor))
	{
		RuntimeActor->SetActorHiddenInGame(true);
		RuntimeActor->SetActorEnableCollision(false);
	}
}

void UQuestTask_GoToLocation::OnActive(EFSMState PreviousState)
{
	if (IsValid(RuntimeActor))
	{
		RuntimeActor->SetActorHiddenInGame(false);
		RuntimeActor->SetActorEnableCollision(true);
	}
}

void UQuestTask_GoToLocation::OnEndActive(EFSMState NextState, EFSMResult Result)
{
	if (IsValid(RuntimeActor))
	{
		RuntimeActor->SetActorHiddenInGame(true);
		RuntimeActor->SetActorEnableCollision(false);
	}
}

void UQuestTask_GoToLocation::OnRestart(EFSMState PreviousState, EFSMResult PreviousResult)
{

}

void UQuestTask_GoToLocation::OnReset()
{
	if (IsValid(RuntimeActor))
	{
		RuntimeActor->OnCompleted.Unbind();
		RuntimeActor->Destroy();
	}
	RuntimeActor = nullptr;

	Super::OnReset();
}

