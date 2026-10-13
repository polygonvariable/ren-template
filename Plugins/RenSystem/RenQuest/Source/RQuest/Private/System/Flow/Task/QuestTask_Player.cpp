// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Flow/Task/QuestTask_Player.h"


void UQuestTask_MovePlayer::CopyFromAsset(const UEventflowTask* Template)
{
	Super::CopyFromAsset(Template);

	const UQuestTask_MovePlayer* Task = Cast<UQuestTask_MovePlayer>(Template);
	checkf(Task, TEXT("Quest task contains invalid task template"));

	Transform = Task->Transform;
}

void UQuestTask_MovePlayer::OnInitialized(EFSMState PreviousState)
{
	Super::OnInitialized(PreviousState);
	Load();
}

void UQuestTask_MovePlayer::OnLoaded(EFSMState PreviousState)
{
	Ready();
}

void UQuestTask_MovePlayer::OnReady(EFSMState PreviousState)
{
	Active();
}

void UQuestTask_MovePlayer::OnActive(EFSMState PreviousState)
{
	UWorld* World = GetWorld();
	APlayerController* Controller = World->GetFirstPlayerController();
	if (!IsValid(Controller))
	{
		ensureMsgf(false, TEXT("Failed to get player controller in quest task"));
		return;
	}

	APawn* Pawn = Controller->GetPawn();
	if (!IsValid(Pawn))
	{
		ensureMsgf(false, TEXT("Failed to get pawn in quest task"));
		return;
	}

	Pawn->SetActorLocation(Transform.GetLocation());
	Pawn->SetActorRotation(Transform.GetRotation());

	Finish(EFSMResult::Success);
}

