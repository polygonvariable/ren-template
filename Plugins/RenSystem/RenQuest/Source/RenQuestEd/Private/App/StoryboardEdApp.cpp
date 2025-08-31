// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "App/StoryboardEdApp.h"

// Engine Headers
#include "Subsystems/AssetEditorSubsystem.h"

// Project Headers
#include "RenEventflowEd/Public/Graph/EventflowEdGraphSchema.h"

#include "RenQuest/Public/QuestAsset.h"
#include "RenQuest/Public/StoryboardAsset.h"

#include "RenQuestEd/Public/Graph/StoryboardEdGraphNode.h"
#include "RenQuestEd/Public/Graph/StoryboardEdGraphSchema.h"



FName FStoryboardEdApp::GetToolkitFName() const
{
	return FName(TEXT("Quest Editor"));
}

FText FStoryboardEdApp::GetBaseToolkitName() const
{
	return FText::FromString(TEXT("Quest Editor"));
}

FString FStoryboardEdApp::GetWorldCentricTabPrefix() const
{
	return FString(TEXT("Quest "));
}

FLinearColor FStoryboardEdApp::GetWorldCentricTabColorScale() const
{
	return FLinearColor::Red;
}

void FStoryboardEdApp::OnGraphNodeDoubleClicked(UEdGraphNode* Node)
{
	if (!Node) return;

	UStoryboardEdQuestNode* QuestNode = Cast<UStoryboardEdQuestNode>(Node);
	if (!QuestNode) return;

	UStoryboardQuestData* NodeData = Cast<UStoryboardQuestData>(QuestNode->GetNodeData());
	if (!NodeData) return;

	UQuestGraph* QuestGraph = NodeData->QuestGraph.LoadSynchronous();
	if (!QuestGraph) return;

	UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
	if (!AssetEditorSubsystem) return;

	AssetEditorSubsystem->OpenEditorForAsset(QuestGraph, EAssetTypeActivationOpenedMethod::Edit);
}

TSubclassOf<UEventflowEdGraphSchema> FStoryboardEdApp::GetGraphSchemaClass() const
{
	return UStoryboardEdGraphSchema::StaticClass();
}

TArray<FName> FStoryboardEdApp::GetTriggerNodeProperties() const
{
	return {
		FName(TEXT("ExtraQuests"))
	};
}

