// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowAsset.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

// Project Headers
#include "RenEventflow/Public/EventflowData.h"
#include "RenEventflow/Public/EventflowNode.h"
#include "RenEventflow/Public/EventflowNodeData.h"
#include "RenEventflow/Public/EventflowBlueprint.h"



#if WITH_EDITOR

void UEventflowAsset::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);
	UE_LOG(LogTemp, Warning, TEXT("UEventflowAsset::PreSave"));
}

void UEventflowAsset::PreSaveRoot(FObjectPreSaveRootContext ObjectSaveContext)
{
	Super::PreSaveRoot(ObjectSaveContext);
	OnEventflowAssetSaved.Broadcast();

	GEngine->ForceGarbageCollection(true);
	UE_LOG(LogTemp, Warning, TEXT("UEventflowAsset::PreSaveRoot"));
}

EDataValidationResult UEventflowAsset::IsDataValid(FDataValidationContext& Context) const
{
	if (!GraphData->Nodes.IsValidIndex(GraphData->EntryNodeIndex))
	{
		Context.AddError(FText::FromString(TEXT("Entry node index is invalid.")));
		return EDataValidationResult::Invalid;
	}

	bool bHasGraphBlueprint = (GraphBlueprint != nullptr);

	const TArray<UEventflowNode*>& GraphNodes = GraphData->Nodes;
	for (UEventflowNode* Node : GraphNodes)
	{
		if (!Node->NodeData)
		{
			Context.AddError(FText::FromString(TEXT("Node data is missing, try resaving the asset.")));
			return EDataValidationResult::Invalid;
		}

		const FName& NodeEvent = Node->NodeData->NodeEvent;
		if (NodeEvent.IsNone())
		{
			continue;
		}

		if (bHasGraphBlueprint)
		{
			UFunction* Function = GraphBlueprint->FindFunctionByName(NodeEvent);
			if (!Function)
			{
				Context.AddError(FText::FromString(TEXT("Node event '" + NodeEvent.ToString() + "' doesn't exist in graph blueprint.")));
				return EDataValidationResult::Invalid;
			}

			TArray<FFieldClass*> ExpectedTypes;
			ExpectedTypes.Add(FObjectProperty::StaticClass());

			bool bMatches = UEventflowBlueprint::HasValidSignature(Function, ExpectedTypes);
			if (!bMatches)
			{
				Context.AddError(FText::FromString(TEXT("Node event '" + NodeEvent.ToString() + "' has invalid signature.")));
				return EDataValidationResult::Invalid;
			}
		}
		else
		{
			if (NodeEvent.IsValid())
			{
				Context.AddError(FText::FromString(TEXT("Graph blueprint is missing but node has event.")));
				return EDataValidationResult::Invalid;
			}
		}
	}

	return EDataValidationResult::Valid;
}

#endif

