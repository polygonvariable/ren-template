// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/QuestObjectiveActor.h"

// Project Headers
#include "EventflowTask.h"
#include "GameplayContextInterface.h"


void AQuestObjectiveActor::SetOwningTask(UEventflowTask* Task)
{
	OwningTask = Task;
}

void AQuestObjectiveActor::StartTasks()
{
	OnStarted.ExecuteIfBound();
}

void AQuestObjectiveActor::AbortTasks()
{
	OnCompleted.ExecuteIfBound(EFSMResult::Aborted);
}

void AQuestObjectiveActor::CompleteTasks()
{
	OnCompleted.ExecuteIfBound(EFSMResult::Success);
}

void AQuestObjectiveActor::CompleteObjective(bool bSuccess)
{
	OnCompleted.ExecuteIfBound(EFSMResult::Success);
}





void AQuestObjectiveActor::HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DoesCollidedWithPlayer(OtherActor) && !bPlayerInRegion)
	{
		OnCompleted.ExecuteIfBound(EFSMResult::Success);
	}
}

void AQuestObjectiveActor::HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	if (DoesCollidedWithPlayer(OtherActor) && bPlayerInRegion)
	{

	}
}







void AQuestObjectiveMarker::HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DoesCollidedWithPlayer(OtherActor) && !bPlayerInRegion)
	{
		OnInteractionCompleted.ExecuteIfBound();
	}
}

void AQuestObjectiveMarker::HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	if (DoesCollidedWithPlayer(OtherActor) && bPlayerInRegion)
	{

	}
}


void AQuestInteractionMarker::BeginPlay()
{
	Super::BeginPlay();
	
	UActorComponent* ContextComponent = FindComponentByInterface(UGameplayContextInterface::StaticClass());
	if (IsValid(ContextComponent))
	{
		IGameplayContextInterface* ContextInterface = Cast<IGameplayContextInterface>(ContextComponent);
		if (ContextInterface)
		{
			TInstancedStruct<FGameplayContextAction> Action;
			FGameplayContext_QuestInteractionHandle& InteractionHandle = Action.InitializeAs<FGameplayContext_QuestInteractionHandle>();
			InteractionHandle.ContextTags = ContextTags;
			InteractionHandle.Owner = TWeakObjectPtr<AQuestInteractionMarker>(this);

			ContextInterface->PushContext(MoveTemp(Action));
		}
	}
}

bool FGameplayContext_QuestInteractionHandle::Execute(UWorld* World, UObject* Caller)
{
	AQuestInteractionMarker* Actor = Owner.Get();
	if (IsValid(Actor))
	{
		Actor->OnInteractionCompleted.ExecuteIfBound();
	}
	return true;
}

