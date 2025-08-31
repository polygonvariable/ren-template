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
#include "RenCoreLibrary/Public/LogMacro.h"

#include "RenEventflow/Public/EventflowAsset.h"
#include "RenEventflow/Public/EventflowBlueprint.h"
#include "RenEventflow/Public/EventflowData.h"
#include "RenEventflow/Public/EventflowEngine.h"
#include "RenEventflow/Public/EventflowNode.h"
#include "RenEventflow/Public/EventflowNodeData.h"
#include "RenEventflow/Public/EventflowNodeTask.h"

#include "RenDialogue/Public/DialogueAsset.h"



void UDialogueOptionWidget::InitializeDetails(FText Option, int Index)
{
	OptionIndex = Index;
	if (OptionText) OptionText->SetText(Option);
}

void UDialogueOptionWidget::SelectOption()
{
	OnOptionSelected.ExecuteIfBound(OptionIndex);
}



void UDialogueWidget::LoadDialogue()
{
	if (DialogueAsset.IsNull()) return;

	EventflowEngine = NewObject<UEventflowEngine>(this);
	EventflowEngine->LoadAsset(DialogueAsset);
	EventflowEngine->OnNodeReached.BindWeakLambda(this, [this](UEventflowNode* Node)
		{
			UDialogueNodeData* NodeData = Cast<UDialogueNodeData>(Node->NodeData);
			if (NodeData)
			{
				if (DialogueTitle) DialogueTitle->SetText(NodeData->DialogueTitle);
				if (DialogueContent) DialogueContent->SetText(NodeData->DialogueContent);
				if (NextButton) NextButton->SetVisibility(ESlateVisibility::Collapsed);

				// Here we currently for testing purposes assume that if a node have an event
				// then it must have a dialogue options.
				// the ideal way would be to create an interface.
				if (!EventflowEngine->StartNodeExecution(Node))
				{
					ShowOptions(Node);
				}
			}
		}
	);
	EventflowEngine->OnNodeExited.BindWeakLambda(this, [this](UEventflowNode* Node, bool bSuccess)
		{
			ShowOptions(Node);
		}
	);
	EventflowEngine->OnGraphEnded.BindWeakLambda(this, [this]()
		{
			UnloadDialogue();
		}
	);
	EventflowEngine->OnEngineInitialized.BindWeakLambda(this, [this]()
		{
			EventflowEngine->ReachEntryNode();
		}
	);
}

void UDialogueWidget::UnloadDialogue()
{
	if (IsValid(EventflowEngine))
	{
		EventflowEngine->OnNodeReached.Unbind();
		EventflowEngine->OnNodeExited.Unbind();
		EventflowEngine->OnGraphEnded.Unbind();

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

	int PinCount = Node->OutputPins.Num();

	const TArray<FText>* Options = Node->NodeData->GetOutputOptions();
	if (!Options) return;

	if (NextButton)
	{
		NextButton->SetVisibility((PinCount <= 1) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

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
	EventflowEngine->ReachImmediateNextNode();
}

void UDialogueWidget::TrySelectOption(int Index)
{
	EventflowEngine->ReachNextNode(Index);
}

void UDialogueWidget::NativeConstruct()
{
	if (NextButton)
	{
		NextButton->OnClicked.AddDynamic(this, &UDialogueWidget::NextDialogue);
	}

	Super::NativeConstruct();
}

void UDialogueWidget::NativeDestruct()
{
	if (NextButton)
	{
		NextButton->OnClicked.RemoveAll(this);
	}

	if (EventflowEngine)
	{
		EventflowEngine->OnNodeReached.Unbind();
	}
	EventflowEngine = nullptr;

	Super::NativeDestruct();
}

