// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialogueEngine.h"

// Project Headers
#include "DialogueNodeData.h"
#include "DialogueWidget.h"
#include "DialogueAsset.h"
#include "Type/EventflowGraphData.h"
#include "Type/EventflowTransition.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"



IDialogueProvider* UDialogueEngine::GetDialogue() const
{
	return Cast<IDialogueProvider>(DialogueWidget);
}

//bool UDialogueEngine::Initialization()
//{
//	if (!Super::Initialization())
//	{
//		return false;
//	}
//
//	UDialogueAsset* Asset = Cast<UDialogueAsset>(CurrentAsset);
//
//	UWorld* World = GetWorld();
//	DialogueWidget = CreateWidget<UDialogueWidget>(World, Asset->DialogueWidgetClass);
//	DialogueWidget->AddToViewport();
//
//	return true;
//}
//
//void UDialogueEngine::Deinitialization()
//{
//	Super::Deinitialization();
//
//	DialogueWidget->RemoveFromParent();
//	DialogueWidget = nullptr;
//}





//void UDialogueTask::CopyFromAsset(const UEventflowTask* Template)
//{
//	const UDialogueTask* DialogueTemplate = Cast<UDialogueTask>(Template);
//	if (IsValid(DialogueTemplate))
//	{
//		DialogueData = DialogueTemplate->DialogueData;
//	}
//}

//TArray<FText> UDialogueTask::GetRuntimeOutputs() const
//{
//	return DialogueData.Options;
//}

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
	//FinishTask(EEventflowTaskState::Completed, NextIndex);
}

//void UDialogueTask::Initialization()
//{
//	IDialogueProvider* Dialogue = GetDialogue();
//	if (!Dialogue)
//	{
//		LOG_ERROR(LogTemp, TEXT("Dialogue is invalid"));
//		return;
//	}
//
//	Dialogue->GetOnDialogueCompleted().BindUObject(this, &UDialogueTask::HandleDialogueCompleted);
//
//	//FInstancedStruct Data = CurrentNode->NodeData;
//	//Dialogue->InitializeDialogue(Data.GetPtr<FDialogueData>());
//	Dialogue->InitializeDialogue(&DialogueData);
//}
//
//void UDialogueTask::Deinitialization()
//{
//	IDialogueProvider* Dialogue = GetDialogue();
//	if (!Dialogue)
//	{
//		LOG_ERROR(LogTemp, TEXT("Dialogue is invalid"));
//		return;
//	}
//	
//	Dialogue->GetOnDialogueCompleted().Unbind();
//	Dialogue->ClearDialogue();
//}
