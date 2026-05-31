// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialogueEngine.h"

// Project Headers
#include "DialogueNodeData.h"
#include "DialogueWidget.h"
#include "EventflowDefinition.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


void UDialogueEngine::StartEngine()
{
	DialogueWidget->SetVisibility(ESlateVisibility::Visible);
	Super::StartEngine();
}

void UDialogueEngine::StopEngine(bool bInterrupted)
{
	DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);
	Super::StopEngine(bInterrupted);
}

IDialogueProvider* UDialogueEngine::GetDialogue() const
{
	return Cast<IDialogueProvider>(DialogueWidget);
}





void UDialogueTask::CopyFromTemplate(const UEventflowTask* Template)
{
	const UDialogueTask* DialogueTemplate = Cast<UDialogueTask>(Template);
	if (IsValid(DialogueTemplate))
	{
		DialogueData2 = DialogueTemplate->DialogueData2;
	}
}

TArray<FText> UDialogueTask::GetRuntimeOutputs() const
{
	return DialogueData2.Options;
}

IDialogueProvider* UDialogueTask::GetDialogue() const
{
	UDialogueEngine* DialogueEngine = GetOwningEngine<UDialogueEngine>();
	if (!IsValid(DialogueEngine))
	{
		return nullptr;
	}
	return Cast<IDialogueProvider>(DialogueEngine->GetDialogue());
}

void UDialogueTask::HandleDialogueCompleted(int NextIndex)
{
	FinishTask(EEventflowDirection::Next, NextIndex);
}

void UDialogueTask::Initialization()
{
	IDialogueProvider* Dialogue = GetDialogue();
	if (!Dialogue)
	{
		LOG_ERROR(LogTemp, TEXT("Dialogue is invalid"));
		return;
	}

	Dialogue->GetOnDialogueCompleted().BindUObject(this, &UDialogueTask::HandleDialogueCompleted);

	//FInstancedStruct Data = CurrentNode->NodeData;
	//Dialogue->InitializeDialogue(Data.GetPtr<FDialogueData>());
	Dialogue->InitializeDialogue(&DialogueData2);
}

void UDialogueTask::Deinitialization()
{
	IDialogueProvider* Dialogue = GetDialogue();
	if (!Dialogue)
	{
		LOG_ERROR(LogTemp, TEXT("Dialogue is invalid"));
		return;
	}
	
	Dialogue->GetOnDialogueCompleted().Unbind();
	Dialogue->ClearDialogue();
}
