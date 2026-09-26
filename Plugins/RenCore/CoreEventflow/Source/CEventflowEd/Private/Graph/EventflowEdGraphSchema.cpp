// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/EventflowEdGraphSchema.h"

// Engine Headers
#include "EdGraph/EdGraphNode.h"

// Project Headers
#include "Graph/EventflowEdGraphNode.h"
#include "Graph/EventflowEdGraphSchemaAction.h"


const FName UEventflowEdGraphSchema::PC_Exec = FName("REN.EF.PC.EXEC");
const FName UEventflowEdGraphSchema::PC_Wildcard = FName("REN.EF.PC.WILDCARD");


void UEventflowEdGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	TMap<FName, UClass*> Classes = GetRegisteredNodeClasses();

	for (const TPair<FName, UClass*>& Kv : Classes)
	{
		UEventflowEdGraphNode* Node = Kv.Value->GetDefaultObject<UEventflowEdGraphNode>();
		if (IsValid(Node))
		{
			FString Category = TEXT("Nodes");
			FText Title = Node->GetNodeTitle(ENodeTitleType::FullTitle);
			FText Description = Node->GetNodeDescription();

			ContextMenuBuilder.AddAction(MakeShareable(new FEventflowEdGraphSchemaAction(Kv.Value, FText::FromString(Category), Title, Description, 0)));
		}
	}
}

const FPinConnectionResponse UEventflowEdGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if (!A || !B)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Can't connect null pins"));
	}

	if (A->Direction == B->Direction)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Can't connect pins of same direction"));
	}

	if (A->GetOwningNode() == B->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Can't connect pins of same node"));
	}

	if (A->Direction == EEdGraphPinDirection::EGPD_Output && B->Direction == EEdGraphPinDirection::EGPD_Input)
	{
		if (A->LinkedTo.Num() > 0)
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT("Can't connect more than one output pin"));
		}
		return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
	}
	
	if (A->Direction == EEdGraphPinDirection::EGPD_Input && B->Direction == EEdGraphPinDirection::EGPD_Output)
	{
		//if (B->LinkedTo.Num() > 0)
		//{
		//	return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT("Can't connect more than one input pin"));
		//}
		return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
	}

	return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Invalid connection condition"));
}

UClass* UEventflowEdGraphSchema::GetRegisteredNodeClass(const FName& ClassName) const
{
	UClass** FoundClass = GetRegisteredNodeClasses().Find(ClassName);
	if (!FoundClass)
	{
		return nullptr;
	}
	return *FoundClass;
}

TMap<FName, UClass*> UEventflowEdGraphSchema::GetRegisteredNodeClasses() const
{
	return TMap<FName, UClass*>();
}

