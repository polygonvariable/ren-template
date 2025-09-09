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

void UEventflowAsset::PostPropertyUpdate()
{
}

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
	FString Message;

	if (!ValidateGraphData(Message) || !ValidateGraphNodes(Message))
	{
		Context.AddError(FText::FromString(Message));
		return EDataValidationResult::Invalid;
	}

	return EDataValidationResult::Valid;
}

bool UEventflowAsset::ValidateGraphData(FString& Message) const
{
	if (!GraphData)
	{
		Message = TEXT("Graph data is missing, try resaving the asset.");
		return false;
	}

	return true;
}

bool UEventflowAsset::ValidateGraphNodes(FString& Message) const
{
	if (!GraphData->NodeList.Contains(GraphData->NodeEntry))
	{
		Message = TEXT("Entry node is invalid.");
		return false;
	}

	bool bHasBlueprint = (GraphBlueprint != nullptr);

	const TMap<FGuid, TObjectPtr<UEventflowNode>>& GraphNodes = GraphData->NodeList;
	for (const auto& Kv : GraphNodes)
	{
		UEventflowNode* Node = Kv.Value.Get();
		if (!Node)
		{
			Message = TEXT("Node is missing, try resaving the asset.");
			return false;
		}

		if (!Node->NodeData)
		{
			Message = TEXT("Node data is missing, try resaving the asset.");
			return false;
		}

		const FName& NodeEvent = Node->NodeData->NodeEvent;
		if (NodeEvent.IsNone())
		{
			continue;
		}

		if (bHasBlueprint)
		{
			UFunction* Function = GraphBlueprint->FindFunctionByName(NodeEvent);
			if (!Function)
			{
				Message = TEXT("Node event '" + NodeEvent.ToString() + "' doesn't exist in graph blueprint, try re-assigning the event.");
				return false;
			}

			TArray<FFieldClass*> ExpectedTypes;
			ExpectedTypes.Add(FObjectProperty::StaticClass());

			bool bMatches = UEventflowBlueprint::HasValidSignature(Function, ExpectedTypes);
			if (!bMatches)
			{
				Message = TEXT("Node event '" + NodeEvent.ToString() + "' has invalid signature, expected: (UEventflowNode* TargetNode).");
				return false;
			}
		}
		else
		{
			if (NodeEvent.IsValid())
			{
				Message = TEXT("Graph blueprint is missing but node has event.");
				return false;
			}
		}
	}

	return true;
}

#endif

