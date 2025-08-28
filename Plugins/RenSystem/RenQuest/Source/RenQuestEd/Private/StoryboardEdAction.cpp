// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "StoryboardEdAction.h"

// Engine Headers

// Project Headers
#include "RenQuest/Public/StoryboardAsset.h"

#include "RenQuestEd/Public/App/StoryboardEdApp.h"



FStoryboardEdAction::FStoryboardEdAction(EAssetTypeCategories::Type InAssetCategory)
{
    AssetCategory = InAssetCategory;
}

FText FStoryboardEdAction::GetName() const
{
    return FText::FromString(TEXT("Storyboard Graph"));
}

FColor FStoryboardEdAction::GetTypeColor() const
{
    return FColor::Cyan;
}

UClass* FStoryboardEdAction::GetSupportedClass() const
{
    return UStoryboardGraph::StaticClass();
}

uint32 FStoryboardEdAction::GetCategories()
{
    return AssetCategory;
}

void FStoryboardEdAction::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
    EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
    for (auto Obj : InObjects)
    {
        UStoryboardGraph* Graph = Cast<UStoryboardGraph>(Obj);
        if (Graph != nullptr)
        {
            TSharedRef<FStoryboardEdApp> App(new FStoryboardEdApp());
			App->InitEditor(Mode, EditWithinLevelEditor, Graph);
        }
    }
}

