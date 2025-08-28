// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowEdAction.h"

// Engine Headers

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"

#include "RenEventflowEd/Public/App/EventflowEdApp.h"



FEventflowEdAction::FEventflowEdAction(EAssetTypeCategories::Type InAssetCategory)
{
    AssetCategory = InAssetCategory;
}

FText FEventflowEdAction::GetName() const
{
    return FText::FromString(TEXT("Eventflow Graph"));
}

FColor FEventflowEdAction::GetTypeColor() const
{
    return FColor::Orange;
}

UClass* FEventflowEdAction::GetSupportedClass() const
{
    return UEventflowAsset::StaticClass();
}

uint32 FEventflowEdAction::GetCategories()
{
    return AssetCategory;
}

void FEventflowEdAction::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
    EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
    for (auto Obj : InObjects)
	{
        UEventflowAsset* Graph = Cast<UEventflowAsset>(Obj);
        if(Graph != nullptr)
		{
            TSharedRef<FEventflowEdApp> App(new FEventflowEdApp());
            App->InitEditor(Mode, EditWithinLevelEditor, Graph);
		}
	}
}

