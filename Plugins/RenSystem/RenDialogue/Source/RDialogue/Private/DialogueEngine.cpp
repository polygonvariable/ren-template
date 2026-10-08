// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialogueEngine.h"

// Project Headers
#include "DialogueAsset.h"
#include "DialogueSettings.h"
#include "DialogueTask.h"
#include "GameplayModeProvider.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "SubsystemLibrary.h"
#include "Task/EventflowNodeTask.h"


void UDialogueEngine::SkipDialogue()
{
	UDialogueAsset* Asset = Cast<UDialogueAsset>(GetAsset());
	check(IsValid(Asset));

	ReachNode(Asset->SkipNodeId);
}

void UDialogueEngine::NextDialogue(int Index)
{
	UDialogueTask_Base* CurrentTask = GetNodeTask<UDialogueTask_Base>();
	if (CurrentTask)
	{
		CurrentTask->ModifyTransitionData(
			[Index](TInstancedStruct<FEventflowNodeTransitionData>& TransitionData)
			{
				FEventflowNodeTransitionData& Data = TransitionData.GetMutable();
				Data.NextNodeIndex = Index;
			}
		);
		CurrentTask->Finish(EFSMResult::Success);
	}
}

void UDialogueEngine::OnReady(EFSMState PreviousState)
{
	UDialogueAsset* Asset = Cast<UDialogueAsset>(GetAsset());
	check(IsValid(Asset));

	IGameplayModeProvider* GameplayMode = FSubsystemLibrary::GetSubsystemInterface<IGameplayModeProvider>(GetWorld());
	if (GameplayMode)
	{
		GameplayMode->PushGameplayMode(UDialogueSettings::Get()->DialogueMode);
	}
}

void UDialogueEngine::OnReset()
{
	IGameplayModeProvider* GameplayMode = FSubsystemLibrary::GetSubsystemInterface<IGameplayModeProvider>(GetWorld());
	if (GameplayMode)
	{
		GameplayMode->PopGameplayMode(UDialogueSettings::Get()->DialogueMode);
	}

	Super::OnReset();
}

