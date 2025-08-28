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
#include "RenEventflow/Public/EventflowAsset.h"
#include "RenEventflow/Public/EventflowData.h"
#include "RenEventflow/Public/EventflowNode.h"
#include "RenEventflow/Public/EventflowNodeData.h"
#include "RenEventflow/Public/EventflowNodeExternalData.h"

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



void UDialogueWidget::LoadAndShowDialogue()
{
	if (DialogueAsset.IsNull()) return;

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(DialogueAsset.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &UDialogueWidget::ShowDialogue));
}

void UDialogueWidget::ShowDialogue()
{
	if (!DialogueAsset || !DialogueAsset->GraphData) return;

	int Index = DialogueAsset->GraphData->EntryNodeIndex;
	if (!DialogueAsset->GraphData->Nodes.IsValidIndex(Index)) return;

	SetCurrentNode(DialogueAsset->GraphData->Nodes[Index]);
}

void UDialogueWidget::ShowOptions()
{
	if (DialogueOptions) DialogueOptions->ClearChildren();

	if (!CurrentNode || !CurrentNode->NodeData || !OptionWidgetClass) return;

	int PinCount = CurrentNode->OutputPins.Num();

	const TArray<FText>* Options = CurrentNode->NodeData->GetOutputOptions();
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
	if (!CurrentNode) return;

	if (CurrentNode->OutputPins.Num() == 0)
	{
		if (NextButton)
		{
			NextButton->SetVisibility(ESlateVisibility::Collapsed);
		}
		UE_LOG(LogTemp, Warning, TEXT("Dialogue end"));
		return;
	}

	if (CurrentNode->OutputPins.Num() > 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Multiple options, waiting for player choice"));
		return;
	}

	SetCurrentNode(CurrentNode->GetNextNodeAt(0));
}

void UDialogueWidget::TrySelectOption(int Index)
{
	if (!CurrentNode) return;
	SetCurrentNode(CurrentNode->GetNextNodeAt(Index));
}

void UDialogueWidget::SetCurrentNode(UEventflowNode* Node)
{
	if (!Node) return;

	CurrentNode = Node;

	UDialogueNodeData* NodeData = Cast<UDialogueNodeData>(CurrentNode->NodeData);
	if (NodeData)
	{
		if (DialogueTitle) DialogueTitle->SetText(NodeData->DialogueTitle);
		if (DialogueContent) DialogueContent->SetText(NodeData->DialogueContent);

		TSubclassOf<UEventflowNodeTask> NodeTask = NodeData->NodeTask;
		if (NodeTask)
		{
			UEventflowNodeTask* Task = NewObject<UEventflowNodeTask>(this, NodeTask);
			Task->StartAction();

			if (Task->GetWaitForCompletion())
			{
				if (DialogueOptions) DialogueOptions->ClearChildren();
				if (NextButton) NextButton->SetVisibility(ESlateVisibility::Collapsed);

				Task->OnActionStopped.AddWeakLambda(this, [this]()
					{
						ShowOptions();
					}
				);
				return;
			}
		}

		ShowOptions();
	}
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
	Super::NativeDestruct();
}

