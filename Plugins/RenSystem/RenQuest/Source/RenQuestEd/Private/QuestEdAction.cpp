// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "QuestEdAction.h"

// Engine Headers
#include "BlueprintEditor.h"

// Project Headers
#include "RenQuest/Public/QuestAsset.h"

#include "RenQuestEd/Public/App/QuestEdApp.h"



FQuestEdAction::FQuestEdAction(EAssetTypeCategories::Type InAssetCategory)
{
    AssetCategory = InAssetCategory;
}

FText FQuestEdAction::GetName() const
{
    return FText::FromString(TEXT("Quest Graph"));
}

FColor FQuestEdAction::GetTypeColor() const
{
    return FColor::Orange;
}

UClass* FQuestEdAction::GetSupportedClass() const
{
    return UQuestGraph::StaticClass();
}

uint32 FQuestEdAction::GetCategories()
{
    return AssetCategory;
}

void FQuestEdAction::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
    EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
    for (auto Obj : InObjects)
    {
        UQuestGraph* Graph = Cast<UQuestGraph>(Obj);
        if (Graph != nullptr)
        {
            TSharedRef<FQuestEdApp> App(new FQuestEdApp());
            App->InitEditor(Mode, EditWithinLevelEditor, Graph);
        }
    }
}

