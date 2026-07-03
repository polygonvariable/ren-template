// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "QuestEdAssetTypeAction.h"

// Project Headers
#include "App/EventflowEdApp.h"
#include "App/QuestEdApp.h"
#include "Data/QuestAsset.h"


FQuestEdAssetTypeAction::FQuestEdAssetTypeAction(EAssetTypeCategories::Type InAssetCategory)
{
    AssetCategory = InAssetCategory;
}

FText FQuestEdAssetTypeAction::GetName() const
{
    return FText::FromString(TEXT("Quest Graph"));
}

FColor FQuestEdAssetTypeAction::GetTypeColor() const
{
    return FColor::Yellow;
}

UClass* FQuestEdAssetTypeAction::GetSupportedClass() const
{
    return UQuestAsset::StaticClass();
}

uint32 FQuestEdAssetTypeAction::GetCategories()
{
    return AssetCategory;
}

void FQuestEdAssetTypeAction::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
    EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
    for (auto Obj : InObjects)
    {
        UQuestAsset* Graph = Cast<UQuestAsset>(Obj);
        if (Graph != nullptr)
        {
            TSharedRef<FQuestEdApp> App(new FQuestEdApp());
			App->InitEditor(Mode, EditWithinLevelEditor, Graph);
        }
    }
}

