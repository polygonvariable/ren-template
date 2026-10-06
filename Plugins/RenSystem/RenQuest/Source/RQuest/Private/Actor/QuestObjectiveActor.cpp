// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/QuestObjectiveActor.h"

// Project Headers
#include "EventflowTask.h"


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




void AQuestInteractionMarker::HandleI()
{
	OnInteractionCompleted.ExecuteIfBound();
}

void AQuestInteractionMarker::HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DoesCollidedWithPlayer(OtherActor) && !bPlayerInRegion)
	{
		EnableInput(GetWorld()->GetFirstPlayerController());
		bPlayerInRegion = true;
	}
}

void AQuestInteractionMarker::HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	if (DoesCollidedWithPlayer(OtherActor) && bPlayerInRegion)
	{
		DisableInput(GetWorld()->GetFirstPlayerController());
		bPlayerInRegion = false;
	}
}
