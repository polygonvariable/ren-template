// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialogueEngine.h"

// Project Headers
#include "DialogueAsset.h"
#include "DialogueSettings.h"
#include "GameplayModeProvider.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Task/EventflowPrimaryTask.h"
#include "Util/SubsystemUtil.h"


void UDialogueEngine::SkipDialogue()
{
	Finish(EFSMResult::Success);
}

void UDialogueEngine::NextDialogue(int Index)
{
	UEventflowPrimaryTask* CurrentTask = GetTask();
	if (CurrentTask)
	{
		CurrentTask->SetTransitionIndex(Index);
		CurrentTask->Finish(EFSMResult::Success);
	}
}


void UDialogueEngine::OnReady(EFSMState PreviousState)
{
	UDialogueAsset* Asset = Cast<UDialogueAsset>(GetAsset());
	if (!IsValid(Asset))
	{
		LOG_ERROR(LogEventflowEngine, TEXT("Failed to get dialogue asset"));
		Finish(EFSMResult::Aborted);
		return;
	}

	IGameplayModeProvider* GameplayMode = FSubsystemLibrary::GetSubsystemInterface<IGameplayModeProvider>(GetWorld());
	if (GameplayMode)
	{
		GameplayMode->PushGameplayMode(UDialogueSettings::Get()->DialogueMode);
	}

	Active();
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

void UDialogueEngine::HandleOnDialogueSkipped()
{
	Finish(EFSMResult::Success);
}

