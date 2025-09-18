// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialogueWidget.h"

// Engine Headers
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenEventflow/Public/EventflowAsset.h"
#include "RenEventflow/Public/EventflowBlueprint.h"
#include "RenEventflow/Public/EventflowData.h"
#include "RenEventflow/Public/EventflowEngine.h"
#include "RenEventflow/Public/EventflowNode.h"
#include "RenEventflow/Public/EventflowNodeData.h"
#include "RenEventflow/Public/EventflowNodeTask.h"

#include "RenDialogue/Public/DialogueAsset.h"
#include "RenDialogue/Public/DialogueNodeData.h"



void UDialogueOptionWidget::InitializeDetails(FText Option, int Index)
{
	OptionIndex = Index;
	if (OptionText) OptionText->SetText(Option);
}

void UDialogueOptionWidget::SelectOption()
{
	OnOptionSelected.ExecuteIfBound(OptionIndex);
}



void UDialogueWidget::SetDialogueContent(UEventflowNode* Node)
{
	if (!Node) return;

	UDialogueNodeData* NodeData = Cast<UDialogueNodeData>(Node->NodeData);
	if (!NodeData) return;

	if (DialogueTitle) DialogueTitle->SetText(NodeData->DialogueTitle);
	if (DialogueContent) DialogueContent->SetText(NodeData->DialogueContent);

	ShowOptions(Node);
}

void UDialogueWidget::LoadDialogue()
{
	if (!DialogueId.IsValid())
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("DialogueId is invalid"));
		return;
	}

	EventflowEngine = NewObject<UEventflowEngine>(this);
	EventflowEngine->LoadAsset(DialogueId);
}

void UDialogueWidget::UnloadDialogue()
{
	if (IsValid(EventflowEngine))
	{
		EventflowEngine->UnloadAsset();
		EventflowEngine->MarkAsGarbage();
	}
	EventflowEngine = nullptr;

	RemoveFromParent();
}

void UDialogueWidget::ShowOptions(UEventflowNode* Node)
{
	if (DialogueOptions) DialogueOptions->ClearChildren();

	if (!Node || !Node->NodeData || !OptionWidgetClass) return;

	int PinCount = Node->NodeOutputs.Num();

	const TArray<FText>* Options = Node->NodeData->GetOutputOptions();
	if (!Options) return;

	if (PinCount <= 1 || PinCount != Options->Num()) return;

	for (int i = 0; i < Options->Num(); i++)
	{
		UDialogueOptionWidget* OptionWidget = CreateWidget<UDialogueOptionWidget>(this, OptionWidgetClass);

		if (!OptionWidget) continue;

		OptionWidget->InitializeDetails((*Options)[i], i);
		OptionWidget->OnOptionSelected.BindUObject(this, &UDialogueWidget::TrySelectOption);

		if (DialogueOptions) DialogueOptions->AddChildToVerticalBox(OptionWidget);
	}
}

void UDialogueWidget::NextDialogue()
{
	// EventflowEngine->ReachImmediateNextNode();
}

void UDialogueWidget::TrySelectOption(int Index)
{
	// EventflowEngine->ReachNextNode(Index);
}

void UDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDialogueWidget::NativeDestruct()
{
	if (EventflowEngine)
	{
		//EventflowEngine->OnNodeReached.Unbind();
	}
	EventflowEngine = nullptr;

	Super::NativeDestruct();
}

