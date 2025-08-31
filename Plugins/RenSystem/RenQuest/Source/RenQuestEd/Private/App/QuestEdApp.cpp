// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "App/QuestEdApp.h"

// Engine Headers

// Project Headers
#include "RenEventflowEd/Public/Graph/EventflowEdGraphSchema.h"

#include "RenQuestEd/Public/Graph/QuestEdGraphSchema.h"



FName FQuestEdApp::GetToolkitFName() const
{
	return FName(TEXT("Quest Editor"));
}

FText FQuestEdApp::GetBaseToolkitName() const
{
	return FText::FromString(TEXT("Quest Editor"));
}

FString FQuestEdApp::GetWorldCentricTabPrefix() const
{
	return FString(TEXT("Quest "));
}

FLinearColor FQuestEdApp::GetWorldCentricTabColorScale() const
{
	return FLinearColor::Red;
}

TSubclassOf<UEventflowEdGraphSchema> FQuestEdApp::GetGraphSchemaClass() const
{
	return UQuestEdGraphSchema::StaticClass();
}

TArray<FName> FQuestEdApp::GetTriggerNodeProperties() const
{
	return {
		FName(TEXT("QuestTask"))
	};
}

