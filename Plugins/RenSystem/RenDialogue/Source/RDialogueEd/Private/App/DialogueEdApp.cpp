// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "App/DialogueEdApp.h"

// Project Headers
#include "Graph/DialogueEdGraphSchema.h"
#include "Graph/EventflowEdGraphSchema.h"


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

TSubclassOf<UEventflowEdGraphSchema> FDialogueEdApp::GetGraphSchemaClass() const
{
	return UDialogueEdGraphSchema::StaticClass();
}

TArray<FName> FDialogueEdApp::GetTriggerNodeProperties() const
{
	return {
		FName(TEXT("OutputOptions")),
		FName(TEXT("DialogueTitle"))
	};
}

