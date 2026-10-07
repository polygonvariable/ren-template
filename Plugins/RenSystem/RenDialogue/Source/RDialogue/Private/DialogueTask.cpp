// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialogueTask.h"

// Project Headers
#include "DialogueEngine.h"


UDialoguePrimaryTask::UDialoguePrimaryTask()
{
	bAllowSubTasks = false;
}

void UDialoguePrimaryTask::OnInitialized(EFSMState PreviousState)
{
	Super::OnInitialized(PreviousState);
	Load();
}

void UDialoguePrimaryTask::OnLoaded(EFSMState PreviousState)
{
	Ready();
}

void UDialoguePrimaryTask::OnReady(EFSMState PreviousState)
{
	Active();
}

void UDialoguePrimaryTask::OnActive(EFSMState PreviousState)
{
	Finish(EFSMResult::Success);
}



UDialogueTask_Begin::UDialogueTask_Begin()
{
	NodeType = EEventflowNodeType::Entry;
}

UDialogueTask_End::UDialogueTask_End()
{
	NodeType = EEventflowNodeType::Exit;
}








void UDialogueTask_Base::CopyFromAsset(const UEventflowTask* Template)
{
	const UDialogueTask_Base* DialogueTemplate = Cast<UDialogueTask_Base>(Template);
	if (IsValid(DialogueTemplate))
	{
		Dialogue = DialogueTemplate->Dialogue;
		Speaker = DialogueTemplate->Speaker;
	}
}

void UDialogueTask_Base::OnInitialized(EFSMState PreviousState)
{
	Super::OnInitialized(PreviousState);
	Load();
}

void UDialogueTask_Base::OnLoaded(EFSMState PreviousState)
{
	Ready();
}

void UDialogueTask_Base::OnReady(EFSMState PreviousState)
{
	Execute();
}

void UDialogueTask_Base::OnActive(EFSMState PreviousState)
{
	UDialogueEngine* DialogueEngine = GetOwningEngine<UDialogueEngine>();
	if (IsValid(DialogueEngine))
	{
		DialogueEngine->OnDialogueContentUpdated.ExecuteIfBound(Dialogue, Speaker);
	}
}

void UDialogueTask_Base::OnFinished(EFSMResult Result)
{
	UDialogueEngine* DialogueEngine = GetOwningEngine<UDialogueEngine>();
	if (IsValid(DialogueEngine))
	{
		DialogueEngine->OnDialogueRemoved.ExecuteIfBound();
	}
}


void UDialogueTask_Branch::CopyFromAsset(const UEventflowTask* Template)
{
	Super::CopyFromAsset(Template);

	const UDialogueTask_Branch* DialogueTemplate = Cast<UDialogueTask_Branch>(Template);
	if (IsValid(DialogueTemplate))
	{
		Options = DialogueTemplate->Options;
	}
}

void UDialogueTask_Branch::OnActive(EFSMState PreviousState)
{
	Super::OnActive(PreviousState);

	UDialogueEngine* DialogueEngine = GetOwningEngine<UDialogueEngine>();
	if (IsValid(DialogueEngine))
	{
		DialogueEngine->OnDialogueOptionsUpdated.ExecuteIfBound(Options);
	}
}

void UDialogueTask_Branch::OnFinished(EFSMResult Result)
{
	Super::OnFinished(Result);
}


