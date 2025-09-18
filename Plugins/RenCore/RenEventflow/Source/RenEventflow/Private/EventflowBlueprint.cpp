// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowBlueprint.h"

// Engine Headers

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"
#include "RenEventflow/Public/EventflowNode.h"
#include "RenEventflow/Public/EventflowNodeData.h"



bool UEventflowBlueprint::StartNodeExecution(UEventflowNode* Node)
{
	if (!Node || !Node->NodeData || Node->NodeData->NodeEvent.IsNone())
	{
		return false;
	}

	UFunction* Function = FindFunction(Node->NodeData->NodeEvent);
	if (!IsValid(Function))
	{
		return false;
	}

	struct FDynamicArgs
	{
		const UEventflowNode* TargetNode;
	};

	FDynamicArgs Args = FDynamicArgs();
	Args.TargetNode = Node;

	ProcessEvent(Function, &Args);

	return true;
}

void UEventflowBlueprint::RegisterBlueprint(UEventflowAsset* Asset)
{
	EventflowAsset = Asset;
}

void UEventflowBlueprint::UnregisterBlueprint()
{
	EventflowAsset.Reset();
}

UWorld* UEventflowBlueprint::GetWorld() const
{
	return GetOuter()->GetWorld();
}

#if WITH_EDITOR

bool UEventflowBlueprint::ImplementsGetWorld() const
{
	return true;
}

#endif

void UEventflowBlueprint::FinishNodeExecution(UEventflowNode* Node, bool bSuccess, int NextNodeIndex)
{
	OnNodeExited.ExecuteIfBound(Node, bSuccess, NextNodeIndex);
}

#if WITH_EDITOR

bool UEventflowBlueprint::HasValidSignature(UFunction* Function, const TArray<FFieldClass*>& ExpectedTypes)
{
	if (!Function) return false;

	int32 Index = 0;

	for (TFieldIterator<FProperty> It(Function); It; ++It)
	{
		FProperty* Prop = *It;

		if (Prop->HasAnyPropertyFlags(CPF_ReturnParm))
		{
			continue;
		}

		if (Index >= ExpectedTypes.Num())
		{
			return false;
		}

		if (!Prop->IsA(ExpectedTypes[Index]))
		{
			return false;
		}

		Index++;
	}

	return Index == ExpectedTypes.Num();
}

#endif

