// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialogueEdAssetTypeAction.h"

// Engine Headers

// Project Headers
#include "RenDialogue/Public/DialogueAsset.h"

#include "RenEventflowEd/Public/App/EventflowEdApp.h"
#include "RenDialogueEd/Public/App/DialogueEdApp.h"



FDialogueEdAssetTypeAction::FDialogueEdAssetTypeAction(EAssetTypeCategories::Type InAssetCategory)
{
    AssetCategory = InAssetCategory;
}

FText FDialogueEdAssetTypeAction::GetName() const
{
    return FText::FromString(TEXT("Dialogue Graph"));
}

FColor FDialogueEdAssetTypeAction::GetTypeColor() const
{
    return FColor::Cyan;
}

UClass* FDialogueEdAssetTypeAction::GetSupportedClass() const
{
    return UDialogueAsset::StaticClass();
}

uint32 FDialogueEdAssetTypeAction::GetCategories()
{
    return AssetCategory;
}

void FDialogueEdAssetTypeAction::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
    EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
    for (auto Obj : InObjects)
    {
        UDialogueAsset* Graph = Cast<UDialogueAsset>(Obj);
        if (Graph != nullptr)
        {
            TSharedRef<FDialogueEdApp> App(new FDialogueEdApp());
			App->InitEditor(Mode, EditWithinLevelEditor, Graph);
        }
    }
}

