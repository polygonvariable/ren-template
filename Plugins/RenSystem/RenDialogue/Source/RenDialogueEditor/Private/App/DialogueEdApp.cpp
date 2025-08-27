// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "App/DialogueEdApp.h"

// Engine Headers

// Project Headers
#include "RenEventflowEditor/Public/Graph/EventflowEdGraph.h"
#include "RenEventflowEditor/Public/Graph/EventflowEdGraphSchema.h"

#include "RenDialogueEditor/Public/Graph/DialogueEdGraph.h"
#include "RenDialogueEditor/Public/Graph/DialogueEdGraphSchema.h"



FName FDialogueEdApp::GetToolkitFName() const
{
	return FName(TEXT("Dialogue Editor"));
}

FText FDialogueEdApp::GetBaseToolkitName() const
{
	return FText::FromString(TEXT("Dialogue Editor"));
}

FString FDialogueEdApp::GetWorldCentricTabPrefix() const
{
	return FString(TEXT("Dialogue "));
}

FLinearColor FDialogueEdApp::GetWorldCentricTabColorScale() const
{
	return FLinearColor::Red;
}



TSubclassOf<UEventflowEdGraph> FDialogueEdApp::GetGraphClass() const
{
	return UDialogueEdGraph::StaticClass();
}

TSubclassOf<UEventflowEdGraphSchema> FDialogueEdApp::GetGraphSchemaClass() const
{
	return UDialogueEdGraphSchema::StaticClass();
}

